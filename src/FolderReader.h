#ifndef __FolderReader_H
#define __FolderReader_H

#include <stack>
#include <string>
#include <windows.h>
using namespace std;

class FolderReader
{
	private:
		stack <string> files;
		string folder_dir;
	public:
		FolderReader(string dir);
		stack<string> getAllFiles();
		string folderDirectory();
	private:
		void displayErrorBox(LPTSTR lpszFunction) ;
};


#endif