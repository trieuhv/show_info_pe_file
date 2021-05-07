##############################
#.SYNOPSIS
# Executes .NET Core test execution (dotnet test) for test projects.
#
#.DESCRIPTION
# Invokes dotnet test for any *.Test.csproj under ..\src.
# By default results will exist in ..\TestResult directory as TRX (mstest) files.
# Interactive tests are excluded by default, use -Filter to change.
#
#.EXAMPLE
# PS:> .\Test.ps1
# Executes tests under ..\src.
#
#.NOTES
# Requires .NET Core SDK with CLI discoverable in $env:Path.
# For information about dotnet test, see https://docs.microsoft.com/en-us/dotnet/core/tools/dotnet-test?tabs=netcore2x.
# For information about mstest v2, see https://github.com/Microsoft/testfx.
##############################
[CmdletBinding()]
param
(
    # Path to search for *.Test.csproj files under to execute tests.
    [ValidateNotNullOrEmpty()]
    [string] $Path = "$PSScriptRoot\..\src",

    # Build configuration to use for discovering tests.
    [ValidateSet('Debug', 'Release')]
    [string] $Configuration = 'Debug',

    # Test case filter. Default is to exclude interactive tests 'TestCategory!=Interactive'.
    [string] $Filter = 'TestCategory!=Interactive',

    # Directory to output results to. Set to $null to not output results. Default is ..\TestResult
    [string] $ResultsDirectory = "$PSScriptRoot\..\TestResult",

    # Type of logger to use. Set to $null to not output results. Default is 'trx'.
    [string] $Logger = 'trx',

    # Indicates verbosity for test runner.
    [ValidateSet('', 'quiet', 'minimal', 'normal', 'detailed', 'diagnostic')]
    [string] $Verbosity,

    # Name of test to execute.
    [string] $TestName,

    # Name of test project to execute.
    [string] $TestProject,

    # Indicates to launch testing to allow attachment of a debugger.
    [switch] $VSTestHostDebug,

    # Indicates to upload test results to AppVeyor.
    [switch] $UploadResultsToAppVeyor
)

Write-Output "Running $($MyInvocation.MyCommand.Name)"
$Path = (Resolve-Path -Path $Path -ErrorAction Stop).ProviderPath

# https://docs.microsoft.com/en-us/dotnet/core/tools/dotnet-test?tabs=netcore2x
$dotnetExe = Get-Command 'dotnet' -ErrorAction Stop

$testProjects = Get-ChildItem -Path $Path -Recurse -Filter '*.Test.csproj'
if(!$testProjects) {
    throw "Failed to find any test projects under: $Path"
}

if($VSTestHostDebug) {
    # Setting this environment variable is necessary for debugging unit tests in Visual Studio Code by instructing 'dotnet test' to wait for a debugger is attached before proceeding.
    # https://github.com/Microsoft/vstest-docs/blob/master/docs/diagnose.md#debug-test-platform-components
    # https://stackoverflow.com/questions/43210794/debugging-mstest-unittests-in-visualstudio-code
    $env:VSTEST_HOST_DEBUG = 1
}

$commonDotnetArgs = @('--configuration', $Configuration, '--no-build', '--no-restore')
if($TestName) {
    # https://docs.microsoft.com/en-us/dotnet/core/testing/selective-unit-tests
    $TestName = "Name~$TestName"
    if($Filter) {
        $Filter = $Filter + "&$TestName"
    }
    else {
        $Filter = $TestName
    }
}

if($Filter) {
    $commonDotnetArgs += @('--filter', "`"$Filter`"")
}

if($ResultsDirectory) {
    # Directory might not exist (which is OK), use GetFullPath instead of Resolve-Path
    $ResultsDirectory = [System.IO.Path]::GetFullPath($ResultsDirectory)
    $commonDotnetArgs += @('--results-directory', "`"$ResultsDirectory`"")
}

if($Verbosity) {
    $commonDotnetArgs += @('--verbosity', "$Verbosity")
}

$testProjects | ForEach-Object {
    if($TestProject -and $_.BaseName -ne $TestProject) {
        return
    }

    $dotnetArgs = @('test', "`"$($_.FullName)`"")
    $dotnetArgs += $commonDotnetArgs

    $logFileName = "$($_.BaseName)_$(Get-Date -Format 'yyyyMMdd_HHmmss').trx"
    if($Logger) {
        $dotnetArgs += @('--logger', "`"$Logger;LogFileName=$logFileName`"")
    }

    Write-Output "Executing: & $dotnetExe $($dotnetArgs -join ' ')"
    & $dotnetExe $dotnetArgs
    $exitCode = $LASTEXITCODE # Save exit code in case upload results fails

    if($ResultsDirectory -and $Logger -and $UploadResultsToAppVeyor -and $env:APPVEYOR_JOB_ID) {
        $resultsLog = Join-Path -Path $ResultsDirectory -ChildPath $logFileName
        if(Test-Path -Path $resultsLog) {
            # https://www.appveyor.com/docs/running-tests/
            try {
                Write-Output "Uploading file to AppVeyor (job ID $($env:APPVEYOR_JOB_ID)): $resultsLog"
                $wc = New-Object 'System.Net.WebClient'
                $wc.UploadFile("https://ci.appveyor.com/api/testresults/mstest/$($env:APPVEYOR_JOB_ID)", $resultsLog)
            }
            catch {
                Write-Warning "Failed to upload test results file: $resultsLog`n$($_ | Out-String)"
            }
        }
        else {
            Write-Warning "No results file found at: $resultsLog"
        }
    }

    if($exitCode -ne 0) {
        throw "Test run failed with exit code: $exitCode"
    }
}

Write-Output "Completed running $($MyInvocation.MyCommand.Name)"
