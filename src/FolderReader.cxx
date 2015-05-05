#include "FolderReader.h"
#include <windows.h>
#include <tchar.h> 
#include <stdio.h>
#include <strsafe.h>
#include <iostream>
#include <stdio.h>

using namespace std;
#pragma comment(lib, "User32.lib")

FolderReader:: FolderReader(string dir)
{
	this->folder_dir = dir;
}

stack<string> FolderReader:: getAllFiles()
{
   stack<string> cloned_files;
   WIN32_FIND_DATA ffd;
   LARGE_INTEGER filesize;
   TCHAR szDir[MAX_PATH];
   size_t length_of_arg;
   HANDLE hFind = INVALID_HANDLE_VALUE;
   DWORD dwError=0;
   stack<string> files;

   // If the directory is not specified as a command-line argument,
   // print usage.
  
/*   if(this->folder_dir.length > 1)
   {
      _tprintf(TEXT("\nUsage: %s <directory name>\n"), "FolderReader");
      return cloned_files;
   }
   */
   // Check that the input path plus 3 is not longer than MAX_PATH.
   // Three characters are for the "\*" plus NULL appended below.

   
   char * folder_c = new char[this->folder_dir.size()+1];
   std::copy(this->folder_dir.begin(), this->folder_dir.end(), folder_c);
   folder_c[folder_dir.size()] = '\0'; // don't forget the terminating 0
   StringCchLength(folder_c, MAX_PATH, &length_of_arg);

   if (length_of_arg > (MAX_PATH - 3))
   {
      _tprintf(TEXT("\nDirectory path is too long.\n"));
      return cloned_files;
   }

   _tprintf(TEXT("\nTarget directory is %s\n\n"), folder_c);

   // Prepare string for use with FindFile functions.  First, copy the
   // string to a buffer, then append '\*' to the directory name.

   StringCchCopy(szDir, MAX_PATH, folder_c);
   StringCchCat(szDir, MAX_PATH, TEXT("\\*"));

   // Find the first file in the directory.

   hFind = FindFirstFile(szDir, &ffd);

   if (INVALID_HANDLE_VALUE == hFind) 
   {
	   cout<<"folder_dir="<<this->folder_dir<<endl;
	  printf("Error at start");
      displayErrorBox(TEXT("FindFirstFile"));
      return cloned_files;
   } 
   
   // List all the files in the directory with some info about them.
	
   string temp_file = "testingg";
   //printf("temp=%s\n", temp_file);
   //cout<<"temp="<<temp_file<<endl;
   //return 1;
   do
   {
      if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
      {
         _tprintf(TEXT("  %s  <DIR>\n"), ffd.cFileName);
      }
      else
      {
         filesize.LowPart = ffd.nFileSizeLow;
         filesize.HighPart = ffd.nFileSizeHigh;

		 temp_file = string(ffd.cFileName);
		 this->files.push(temp_file);
		//printf("wc=%s\n", wc);
		//printf("wc[0]=%c\n",wc[0]);
		//printf("*wc=%c\n",*wc);
    //convert from wide char to narrow char array
    /**char ch[260];
    char DefChar = ' ';
    WideCharToMultiByte(CP_ACP,0,wc,-1, ch,260,&DefChar, NULL);
    
    //A std:string  using the char* constructor.
    std::string ss(ch);
	**/
		 //files.push(string(ffd.cFileName));
		 //temp_file = narrow(string(ffd.cFileName));		 
		 //printf("%s \n", temp_file);	
	}
   }
   while (FindNextFile(hFind, &ffd) != 0);
 
   dwError = GetLastError();
   if (dwError != ERROR_NO_MORE_FILES) 
   {
	   cout<<"folder_dir="<<this->folder_dir<<endl;
	   printf("Error_no_more_files");
      displayErrorBox(TEXT("FindFirstFile"));
   }

   FindClose(hFind);

   //cout<<"temp_file"<<temp_file<<endl;
   cout<<"Numbe_Files:"<<this->files.size()<<endl;
   cloned_files = files;
   return (this->files);
}

string FolderReader:: folderDirectory()
{
	return this->folder_dir;
}

void FolderReader:: displayErrorBox(LPTSTR lpszFunction) 
{ 
    // Retrieve the system error message for the last-error code

    LPVOID lpMsgBuf;
    LPVOID lpDisplayBuf;
    DWORD dw = GetLastError(); 

    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | 
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dw,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR) &lpMsgBuf,
        0, NULL );

    // Display the error message and clean up

    lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT, 
        (lstrlen((LPCTSTR)lpMsgBuf)+lstrlen((LPCTSTR)lpszFunction)+40)*sizeof(TCHAR)); 
    StringCchPrintf((LPTSTR)lpDisplayBuf, 
        LocalSize(lpDisplayBuf) / sizeof(TCHAR),
        TEXT("%s failed with error %d: %s"), 
        lpszFunction, dw, lpMsgBuf); 
    MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK); 

    LocalFree(lpMsgBuf);
    LocalFree(lpDisplayBuf);
}
