
// MFC_PEFileDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MFC_PEFile.h"
#include "MFC_PEFileDlg.h"
#include "afxdialogex.h"
#include "afxtaskdialog.h"
#include <string>
#include <iostream>
#include<windows.h>
#include<time.h>
#include<tchar.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFC_PEFileDlg dialog



CMFC_PEFileDlg::CMFC_PEFileDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMFC_PEFileDlg::IDD, pParent)
{
	
}

void CMFC_PEFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_FILE_PLACE, m_file_place);
	DDX_Control(pDX, IDC_BUTTON_BROWN, m_btn_brown);
	DDX_Control(pDX, IDC_EDIT_DOS_IFANEW, m_edit_dos_ifanew);
	DDX_Control(pDX, IDC_EDIT_DOS_MAGIC, m_edit_dos_magic);
	DDX_Control(pDX, IDC_BUTTON_DOS, m_btn_dos);
	DDX_Control(pDX, IDC_BUTTON_PE, m_btn_pe);
	DDX_Control(pDX, IDC_EDIT_PE_SIGNATURE, m_edit_pe_signature);
	DDX_Control(pDX, IDC_EDIT_PE_MACHINE, m_edit_pe_machine);
	DDX_Control(pDX, IDC_EDIT_PE_CHARACTERISTICS, m_edit_pe_characteristic);
	DDX_Control(pDX, IDC_EDIT_PE_NOS, m_edit_pe_nos);
	DDX_Control(pDX, IDC_EDIT_OP_AOEP, m_edit_op_aoep);
	DDX_Control(pDX, IDC_EDIT_OP_SA, m_edit_op_sa);
	DDX_Control(pDX, IDC_EDIT_OP_IB, m_edit_op_ib);
	DDX_Control(pDX, IDC_EDIT_OP_FA, m_edit_op_fa);
	DDX_Control(pDX, IDC_BUTTON_OP, m_btn_op);
	DDX_Control(pDX, IDC_EDIT_OP_RVA, m_edit_op_rva);
	DDX_Control(pDX, IDC_BUTTON_SECTION, m_btn_se);
	DDX_Control(pDX, IDC_EDIT_SE_NAMEL, m_edit_se_namel);
	DDX_Control(pDX, IDC_EDIT_SE_VS, m_edit_se_vs);
	DDX_Control(pDX, IDC_EDIT_SE_VA, m_edit_se_va);
	DDX_Control(pDX, IDC_EDIT_SE_SORD, m_edit_se_sord);
	DDX_Control(pDX, IDC_EDIT_SE_CHARACTORISTICS, m_edit_se_charactoristics);
	DDX_Control(pDX, IDC_EDIT_SE_PTRD, m_edit_se_ptrd);
}

BEGIN_MESSAGE_MAP(CMFC_PEFileDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_BROWN, &CMFC_PEFileDlg::OnClickBtnBrown)
	ON_BN_CLICKED(IDC_BUTTON_DOS,&CMFC_PEFileDlg::OnClickBtnDOS)
	ON_BN_CLICKED(IDC_BUTTON_PE,&CMFC_PEFileDlg::OnClickBtnPE)
	ON_BN_CLICKED(IDC_BUTTON_OP,&CMFC_PEFileDlg::OnClickBtnOP)
END_MESSAGE_MAP()


// CMFC_PEFileDlg message handlers

BOOL CMFC_PEFileDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMFC_PEFileDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMFC_PEFileDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMFC_PEFileDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMFC_PEFileDlg::OnClickBtnBrown()

{

		
		 // szFilters is a text string that includes two file name filters:
   // "*.my" for "MyType Files" and "*.*' for "All Files."
   TCHAR szFilters[]= _T("MyType Files (*.exe)|*.exe|All Files (*.*)|*.*||");

   // Create an Open dialog; the default file name extension is ".my".
   CFileDialog fileDlg(TRUE, _T("my"), _T("*.exe"),
      OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilters);
   
   // Display the file dialog. When user clicks OK, fileDlg.DoModal() 
   // returns IDOK.

      

   if(fileDlg.DoModal() == IDOK)
   {
       pathName = fileDlg.GetPathName();
	   hang=1;
      // Implement opening and reading file in here.
      //Change the window's title to the opened file's title.
	   m_file_place.SetWindowTextW(pathName);
		// Open the binary file.
	   hFile = CreateFile(pathName,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	   hMapObject = CreateFileMapping(hFile,NULL,PAGE_READONLY,0,0,NULL);
	   lpBase = MapViewOfFile(hMapObject,FILE_MAP_READ,0,0,0);
	   dosHeader = (PIMAGE_DOS_HEADER)lpBase;// 0x04000000
//DOS header------------------------------------------------------------------------
	    if(dosHeader->e_magic == IMAGE_DOS_SIGNATURE){
                        //Dump the Dos Header info
			long x,y;
			x=dosHeader->e_magic;
			y=dosHeader->e_lfanew;
			CString t;
			t.Format(_T("%#x"),x);
			m_edit_dos_magic.SetWindowTextW(t);
			t.Format(_T("%#x"),y);
			m_edit_dos_ifanew.SetWindowTextW(t);
		
		}
//PE header---------------------------------------------------------------------------
		 //Offset of NT Header is found at 0x3c location in DOS header specified by e_lfanew
         //Get the Base of NT Header(PE Header)  = dosHeader + RVA address of PE header
          ntHeader = (PIMAGE_NT_HEADERS)((DWORD)(dosHeader) + (dosHeader->e_lfanew));
                //Identify for valid PE file  
			if(ntHeader->Signature == IMAGE_NT_SIGNATURE){
				long x,y,z;
				
				//show signature
				x=ntHeader ->Signature;
				CString t;
				t.Format(_T("%#x"),x);
				m_edit_pe_signature.SetWindowTextW(t);
				m_edit_pe_machine.LineScroll(4);
				//show machine
				header=ntHeader->FileHeader;
				switch (header.Machine){
					case 0x0:    m_edit_pe_machine.SetWindowTextW(_T("0x0 - All"));break;
					case 0x14d:  m_edit_pe_machine.SetWindowTextW(_T("0x14d - Intel i860")); break;
					case 0x14c:  m_edit_pe_machine.SetWindowTextW(_T("0x14c - Intel i386,\r\ni486,i586")); break;
					case 0x200:   m_edit_pe_machine.SetWindowTextW(_T("0x200 - Intel \r\n Itanium processor")); break;
					case 0x8664:  m_edit_pe_machine.SetWindowTextW(_T("0x8664 - AMD x64")); break;
                    case 0x162:   m_edit_pe_machine.SetWindowTextW(_T("0x162 - MIPS R3000")); break;
                    case 0x166:   m_edit_pe_machine.SetWindowTextW(_T("0x166 - MIPS R4000")); break;
                    case 0x183:   m_edit_pe_machine.SetWindowTextW(_T("0183 - DEC Alpha AXP")); break;
                    default:      m_edit_pe_machine.SetWindowTextW(_T("Not Found")); break;
				
				
				}

				// show characteristics

				if((header.Characteristics&0x0002) == 0x0002)
					m_edit_pe_characteristic.SetWindowTextW(_T("\n Executable\r\n Image"));
                if((header.Characteristics&0x0020) == 0x0020) 
					m_edit_pe_characteristic.SetWindowTextW(_T("\n Application \r\n address > 2GB"));
                if((header.Characteristics&0x1000) == 0x1000)  
					m_edit_pe_characteristic.SetWindowTextW(_T("\nSystem file \r\n(Kernel Mode Driver)"));
                if((header.Characteristics&0x2000) == 0x2000)  
					m_edit_pe_characteristic.SetWindowTextW(_T("\nDll file"));
                if((header.Characteristics&0x4000) == 0x4000)  
					m_edit_pe_characteristic.SetWindowTextW(_T("\nApplication runs \r\n only in Uniprocessor ,"));

			// show number of sections
				t.Format(_T("%d"),header.NumberOfSections);
		     	m_edit_pe_nos.SetWindowTextW(t);
//Option Header ------------------------------------------------------------------	
				opHeader = ntHeader->OptionalHeader;
				 CString aoep,sa,ib,fa,rva;
				 //show adress of entry point
				 aoep.Format(_T("%#x"),opHeader.AddressOfEntryPoint);
				 m_edit_op_aoep.SetWindowTextW(aoep);

			 // show Image Base
				 ib.Format(_T("%#x"),opHeader.ImageBase);
				 m_edit_op_ib.SetWindowTextW(ib);

			 // show section alignment
				 sa.Format(_T("%#x"),opHeader.SectionAlignment);
				 m_edit_op_sa.SetWindowTextW(sa);

			 // show File alignment
				 fa.Format(_T("%#x"),opHeader.FileAlignment);
				 m_edit_op_fa.SetWindowTextW(fa);

			 // show RVA
				 rva.Format(_T("%#x"),opHeader.BaseOfData);
				 m_edit_op_rva.SetWindowTextW(rva);
				
//Section table -----------------------------------------------------------		
				 pSecHeader = IMAGE_FIRST_SECTION(ntHeader);
				 CString tt;
				for(int i=0;i<ntHeader->FileHeader.NumberOfSections;i++,pSecHeader++){  
                              
                             //   printf("\n%-36s%s","Section Header name : ", pSecHeader->Name);
								tt.Format(_T("%#x"),pSecHeader->Name);
								m_edit_se_namel.SetWindowTextW(tt);
                              //  printf("\n%-36s%#x","ActualSize of code or data : ", pSecHeader->Misc.VirtualSize);
								tt.Format(_T("%#x"),pSecHeader->SizeOfRawData);
								m_edit_se_sord.SetWindowTextW(tt);
                              ///  printf("\n%-36s%#x","Virtual Address(RVA) :", pSecHeader->VirtualAddress);
								tt.Format(_T("%#x"),pSecHeader->VirtualAddress);
								m_edit_se_va.SetWindowTextW(tt);
                               // printf("\n%-36s%#x","Size of raw data (rounded to FA) : ", pSecHeader->SizeOfRawData);
								tt.Format(_T("%#x"),pSecHeader->PointerToRawData);
								m_edit_se_ptrd.SetWindowTextW(tt);

								tt.Format(_T("%#x"),pSecHeader->Misc.VirtualSize);
								m_edit_se_vs.SetWindowTextW(tt);

								//tt.Format(_T("%#x"),pSecHeader->VirtualAddress);


                            //    printf("\n%-36s%#x","Pointer to Raw Data : ", pSecHeader->PointerToRawData);
                              //  printf("\n%-36s%#x","Pointer to Relocations : ", pSecHeader->PointerToRelocations);
                              //  printf("\n%-36s%#x","Pointer to Line numbers : ", pSecHeader->PointerToLinenumbers);
                             //   printf("\n%-36s%#x","Number of relocations : ", pSecHeader->NumberOfRelocations);
                            //    printf("\n%-36s%#x","Number of line numbers : ", pSecHeader->NumberOfLinenumbers);
                             //   printf("\n%-36s%s","Characteristics : ","Contains ");
                                if((pSecHeader->Characteristics&0x20)==0x20)//"executable code"
								m_edit_se_charactoristics.SetWindowTextW(_T("executable code, "));
                                if((pSecHeader->Characteristics&0x40)==0x40)//"initialized data"
									m_edit_se_charactoristics.SetWindowTextW(_T("executable code, initialized data"));
                                if((pSecHeader->Characteristics&0x80)==0x80)//"uninitialized data"
								m_edit_se_charactoristics.SetWindowTextW(_T("executable code, initialized data,uninitialized data"));
                              
                                if((pSecHeader->Characteristics&0x200)==0x200)//"comments and linker commands"
								m_edit_se_charactoristics.SetWindowTextW(_T("executable code, initialized data,uninitialized data,comments and linker commands"));
                                if((pSecHeader->Characteristics&0x10000000)==0x10000000)//"shareable data(via DLLs)"
								m_edit_se_charactoristics.SetWindowTextW(_T("executable code, initialized data,uninitialized data,comments and linker commands,shareable data(via DLLs)"));
                                if((pSecHeader->Characteristics&0x40000000)==0x40000000)//"Readable"
									m_edit_se_charactoristics.SetWindowTextW(_T("executable code, initialized data,uninitialized data,comments and linker commands,shareable data(via DLLs),Readable"));
                                if((pSecHeader->Characteristics&0x80000000)==0x80000000)//"Writable"
									m_edit_se_charactoristics.SetWindowTextW(_T("executable code, initialized data,uninitialized data,comments and linker commands,shareable data(via DLLs),Readable,WriteAble !"));
                               
				}
      	}



			










			

	}else
	{
		return;
	}


 } 


void CMFC_PEFileDlg:: OnClickBtnDOS(){
	
	if(CTaskDialog::IsSupported()&&hang==1){
		CString Title(_T("More Information of DOS Header"));
		CString Heading("DOS Header !!!");
		CString Content("");
		
	   hFile = CreateFile(pathName,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	   hMapObject = CreateFileMapping(hFile,NULL,PAGE_READONLY,0,0,NULL);
	   lpBase = MapViewOfFile(hMapObject,FILE_MAP_READ,0,0,0);
	   dosHeader = (PIMAGE_DOS_HEADER)lpBase;

	    if(dosHeader->e_magic == IMAGE_DOS_SIGNATURE){

			long a,b,c,d,e,f,g,h,i,k,l,m,n,o,p,q,r;
			CString ta,tb,tc,te,td,tf,tg,th,ti,tk,tl,tm,tn,to,tp,tq,tr;
			a=dosHeader->e_magic;
			b=dosHeader->e_lfanew;
			c=dosHeader->e_cblp;
			d=dosHeader->e_cp;
			e=dosHeader->e_crlc;
			f=dosHeader->e_cparhdr;
			g=dosHeader->e_minalloc;
			h=dosHeader->e_maxalloc;
			i=dosHeader->e_ss;
			k=dosHeader->e_sp;
			l=dosHeader->e_csum;
			m=dosHeader->e_ip;
			n=dosHeader->e_cs;
			o=dosHeader->e_lfarlc;
			p=dosHeader->e_ovno;
			q=dosHeader->e_oemid;
			r=dosHeader->e_oeminfo;
			ta.Format(_T("%#x"),a);
			tb.Format(_T("%#x"),b);
			tc.Format(_T("%#x"),c);
			td.Format(_T("%#x"),d);
			te.Format(_T("%#x"),e);
			tf.Format(_T("%#x"),f);
			tg.Format(_T("%#x"),g);
			th.Format(_T("%#x"),h);
			ti.Format(_T("%#x"),i);
			tk.Format(_T("%#x"),k);
			tl.Format(_T("%#x"),l);
			tm.Format(_T("%#x"),m);
			tn.Format(_T("%#x"),n);
			to.Format(_T("%#x"),+o);
			tp.Format(_T("%#x"),+p);
			tq.Format(_T("%#x"),q);
			tr.Format(_T("%#x"),r);
			ta=_T("Magic number : ")+ta;
			tb=_T("\nRVA address of PE header : ")+tb;
			tc=_T("\nBytes on last page of file :")+tc;
			td=_T("\nPages in file : ")+td;
			te=_T("\nRelocation : ")+te;
			tf=_T("\nSize of header in paragraphs : ")+tf;
			tg=_T("\nMinimum extra paragraphs needed : ")+tg;
			th=_T("\nMaximum extra paragraphs needed : ")+th;
			ti=_T("\nInitial (relative) SS value : ")+ti;
			tk=_T("\nInitial SP value : ")+tk;
			tl=_T("\nChecksum : ")+tl;
			tm=_T("\nInitial IP value : ")+tm;
			tn=_T("\nInitial (relative) CS value : ")+tn;
			to=_T("\nFile address of relocation table : ")+to;
			tp=_T("\nOverlay number : ")+tp;
			tq=_T("\nOEM identifier : ")+tq;
			tr=_T("\nOEM information(e_oemid specific) :")+tr;

			Content=ta+tb+tc+td+te+tf+tg+th+ti+tk+tl+tm+tn+to+tp+tq+tr;
			
		}
		
		CTaskDialog taskDlg(Content,Heading,Title,TDCBF_OK_BUTTON);
		taskDlg.DoModal();


	
	}





	       
}
void CMFC_PEFileDlg:: OnClickBtnPE(){

	if(CTaskDialog::IsSupported()&&hang==1){
		CString Title(_T("More Information of PE Header(File header)"));
		CString Heading("PE Header !!!");
		CString Content("");
		
	   hFile = CreateFile(pathName,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	   hMapObject = CreateFileMapping(hFile,NULL,PAGE_READONLY,0,0,NULL);
	   lpBase = MapViewOfFile(hMapObject,FILE_MAP_READ,0,0,0);
	   dosHeader = (PIMAGE_DOS_HEADER)lpBase;
	     ntHeader = (PIMAGE_NT_HEADERS)((DWORD)(dosHeader) + (dosHeader->e_lfanew));
		 if(ntHeader->Signature == IMAGE_NT_SIGNATURE){
		 long x;
		 //show signature
				x=ntHeader ->Signature;
				CString signature,nos,machine,characteristics,nox,soo,time;
				signature.Format(_T("%#x"),x);
				signature= CString("Signature : ")+signature+CString(" - PE");
				//show machine
				header=ntHeader->FileHeader;
				switch (header.Machine){
					case 0x0:    machine="0x0 - All";break;
					case 0x14d:  machine="0x14d - Intel i860"; break;
					case 0x14c:  machine="0x14c - Intel i386,i486,i586"; break;
					case 0x200:  machine="0x200 - Intel Itanium processor"; break;
					case 0x8664: machine="0x8664 - AMD x64"; break;
                    case 0x162:  machine="0x162 - MIPS R3000"; break;
                    case 0x166:  machine="0x166 - MIPS R4000"; break;
                    case 0x183:  machine="0183 - DEC Alpha AXP"; break;
                    default:     machine="Not Found"; break;
				
				
				}
				machine =CString ("\nMachine Architechture : ") +machine;

				// show characteristics

				if((header.Characteristics&0x0002) == 0x0002)
					characteristics="Executable Image";
                if((header.Characteristics&0x0020) == 0x0020) 
					characteristics="Application address > 2GB";
                if((header.Characteristics&0x1000) == 0x1000)  
					characteristics="System file(Kernel Mode Driver)";
                if((header.Characteristics&0x2000) == 0x2000)  
					characteristics="Dll file";
                if((header.Characteristics&0x4000) == 0x4000)  
					characteristics="Application runs only in Uniprocessor";
				characteristics=CString ("\n Characteristics : ")+characteristics;
			// show number of sections
				nos.Format(_T("%d"),header.NumberOfSections);
		     	nos = CString ("\nNumber Of Sections : ")+ nos;
			// show number of symbols

				nox.Format(_T("%d",header.NumberOfSymbols));
				nox = CString ("\nNumber Of Symbols : ") +nox;
			// show size of optional header

				soo.Format(_T("%d"),header.SizeOfOptionalHeader);
				soo = CString ("\nSize Of Optional Header : ")+soo+CString (" Bytes");
			// show time stamp
			//	time.Format(_T("%s"),header.TimeDateStamp);
				//time =CString ("\n Date and Time : ")+time;

				Content= signature+machine+characteristics+nos+nox+ soo+time;
		 
		 
		 }
		
		CTaskDialog taskDlg(Content,Heading,Title,TDCBF_OK_BUTTON);
		taskDlg.DoModal();








}

}
void CMFC_PEFileDlg:: OnClickBtnOP(){
	if(CTaskDialog::IsSupported()&&hang==1){
	CString Title(_T("More Information of PE Header(Optional header)"));
		CString Heading("Optional Header !!!");
		CString Content("");
		CString InforDataDirectory;
		
	   hFile = CreateFile(pathName,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	   hMapObject = CreateFileMapping(hFile,NULL,PAGE_READONLY,0,0,NULL);
	   lpBase = MapViewOfFile(hMapObject,FILE_MAP_READ,0,0,0);
	   dosHeader = (PIMAGE_DOS_HEADER)lpBase;
	   ntHeader = (PIMAGE_NT_HEADERS)((DWORD)(dosHeader) + (dosHeader->e_lfanew));
	   
		 if(ntHeader->Signature == IMAGE_NT_SIGNATURE){
			 opHeader = ntHeader->OptionalHeader;
				 CString aoep,sa,ib,fa,rva,a,b,c,d,e,f,g,h,i,k,l,m;
				 CString aa,bb;
				 CString cc[20],dd[20];
				 //show adress of entry point
				 aoep.Format(_T("%#x"),opHeader.AddressOfEntryPoint);
				 aoep=CString("Address of Entry Point : ")+aoep;
				 
			 // show Image Base
				 ib.Format(_T("%#x"),opHeader.ImageBase);
				 ib=CString("\nBase Address of the Image : ")+ib;

			 // show section alignment
				 sa.Format(_T("%#x"),opHeader.SectionAlignment);
				sa=CString("\nSection Alignment : ")+sa;

			 // show File alignment
				 fa.Format(_T("%#x"),opHeader.FileAlignment);
				fa=CString("\nFile Alignment : ")+fa;
				// show rva
				 rva.Format(_T("%#x"),opHeader.BaseOfData);
				 rva=CString("\nBase Adrress of Data Segment : ")+rva;
				 // show size of code
				 a.Format(_T("%#x"),opHeader.SizeOfCode);
				 a =CString("\nSize of Code Segment (*.text) : ")+a;
				 // show base of code
				 b.Format(_T("%#x"),opHeader.BaseOfCode);
				 b=CString ("\nBase Adress of Code Segment : ")+b;
				 //show directory
				   c.Format(_T("%#x"),opHeader.DataDirectory);
				   c=CString ("\nData Directory : ")+c;
				//show size of initialized data
				    d.Format(_T("%#x"),opHeader.SizeOfInitializedData);
					d=CString ("\nSize of Initialized Data : ")+d;	
				// Major Linker Version 
					e.Format(_T("%#x"),opHeader.MajorLinkerVersion);
					e=CString ("\nMajor Linker Version : ")+e;
				//Minor Linker Version 
					f.Format(_T("%#x"),opHeader.MinorLinkerVersion);
					f =CString ("\nMinor Linker Version : ")+f;
				//Size of image
					g.Format(_T("%#x"),opHeader.SizeOfImage);
					g =CString ("\nSize of Image : ")+g;
				// Size of Header 
					h.Format(_T("%#x"),opHeader.SizeOfHeaders);
					h =CString ("\nSize of Headers : ")+h;
					// show dll charactoristics
					i.Format(_T("%#x"),opHeader.DllCharacteristics);
					i =CString ("\nDllCharacteristics : ")+i;
				// show loader flags
					k.Format(_T("%#x"),opHeader.LoaderFlags);
					k =CString ("\nLoaderFlags : ")+k;
					// get magic
					if(opHeader.Magic==0x20b)
					l="\nGiven file is a : PE32+(64)";
					else
					l="\nGiven file is a : PE32";
					//show process
					if(opHeader.Subsystem==1)
						m="\nSubSystem type : Device Driver(Native windows Process)";
					if(opHeader.Subsystem==2)
						m="\nSubSystem type : Windowns GUI";
					if(opHeader.Subsystem==3)
						m="\nSubSystem type : Windowns CLI";
					if(opHeader.Subsystem==9)
						m="\nSubSystem type : Windown CE GUI";
					Content=aoep+sa+ib+fa+rva+a+b+c+d+e+f+g+h+i+k+l+m;
//Data Directory---------
					
				
					
					for(int i=0;i<16;i++){
					aa.Format(_T("%#x"),opHeader.DataDirectory[i].VirtualAddress);
					cc[i]=aa;
					bb.Format(_T("%#x"),opHeader.DataDirectory[i].Size);
					dd[i]=bb;
					
					if(i==15)
					InforDataDirectory=CString("Export Table -Addr : ")+cc[0]
									+	CString(" - Size : ")+dd[0]
									+   CString("\nImport Table -Addr : ")+cc[1]
									+	CString(" - Size : ")+dd[1]
									+   CString("\nResource -Addr : ")+cc[2]
									+	CString(" - Size : ")+dd[2]
									+   CString("\nSecurity -Addr : ")+cc[3]
									+	CString(" - Size : ")+dd[3]
									+   CString("\nRelocation -Addr : ")+cc[4]
									+	CString(" - Size : ")+dd[4]
									+   CString("\nBasereloc -Addr : ")+cc[5]
									+	CString(" - Size : ")+dd[5]
									+   CString("\nDebug -Addr : ")+cc[6]
									+	CString(" - Size : ")+dd[6]
									+   CString("\nCopyright -Addr : ")+cc[7]
									+	CString(" - Size : ")+dd[7]
									+   CString("\nGlobalPTR -Addr : ")+cc[8]
									+	CString(" - Size : ")+dd[8]
									+   CString("\nTLS : ")+cc[9]
									+	CString(" - Size : ")+dd[9]
									+   CString("\nLoad Config -Addr : ")+cc[10]
									+	CString(" - Size : ")+dd[10]
									+   CString("\nBound Import -Addr : ")+cc[11]
									+	CString(" - Size : ")+dd[11]
									+   CString("\nIAT -Addr : ")+cc[12]
									+	CString(" - Size : ")+dd[12]
									+   CString("\nDelay Import -Addr : ")+cc[13]
									+	CString(" - Size : ")+dd[13]
									+   CString("\nCom Descriptor -Addr : ")+cc[14]
									+	CString(" - Size : ")+dd[14]
									+   CString("\nNumber of Directory Entries -Addr : ")+cc[15]
									+	CString(" - Size : ")+dd[15];
								
									
					}
		 }
		
		CTaskDialog taskDlg(Content,Heading,Title,TDCBF_OK_BUTTON);
		taskDlg.SetExpansionArea(InforDataDirectory,_T("DataDirectory detail"),_T("Ngoc than thanh"));
		taskDlg.DoModal();

		}

}