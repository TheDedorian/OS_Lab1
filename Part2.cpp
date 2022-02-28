#include <Windows.h>
#include <stdio.h>
#include <iostream>

using namespace std;

int callback = 0;

HANDLE CreateFile_1() //создание первого файла для чтения
{ 
    LPCWSTR  name;
    string select_file;
    cout << "Select a first file (path):" << endl;
    cin >> select_file;
    name =  (LPCWSTR) (select_file.c_str());
    HANDLE hfile = CreateFile((LPCSTR) name, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED | FILE_FLAG_NO_BUFFERING, NULL);
    if (hfile != INVALID_HANDLE_VALUE)
    {
        cout << "File was created" << endl;
        return hfile;
    }
    else
    {
        cout << "Error" << endl;
        return NULL;
    }
}

HANDLE CreateFile_2() //создание второго файла для записи
{
    LPCWSTR  name;
    string select_file;
    cout << "Select a second file (path):" << endl;
    cin >> select_file;
    name =  (LPCWSTR) (select_file.c_str());
    HANDLE hfile_2 = CreateFile((LPCSTR) name,  GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_FLAG_OVERLAPPED | FILE_FLAG_NO_BUFFERING, NULL);
    if (hfile_2 != INVALID_HANDLE_VALUE)
    {
        cout << "File was created" << endl;
        return hfile_2;
    }

    else
    {
        cout << "Error" << endl;
        return NULL;
    }
}

VOID CALLBACK FileIOCompletionRoutine(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered, LPOVERLAPPED lpOverlapped)
{
    callback++;
}

void CopyFile(HANDLE hfile_1, HANDLE hfile_2, DWORD size_copy, int operationsCount){ 
           DWORD fileSizeHigh = 0;
           DWORD fileSizeLow = GetFileSize(hfile_1, &fileSizeHigh);
           LARGE_INTEGER fileSize = {};
           fileSize.LowPart = fileSizeLow;
           fileSize.HighPart = fileSizeHigh;
           LARGE_INTEGER Copy_Size_Data = {};
           LARGE_INTEGER Data_size = {};
           Data_size.QuadPart = ( (fileSize.QuadPart / size_copy + (fileSize.QuadPart % size_copy > 0 ? 1:0) ) * size_copy );
           int Count_Op;
           BOOL Status;
           LARGE_INTEGER Size_Write_Data = {};
           CHAR* buf = new CHAR[ operationsCount*size_copy ];
           OVERLAPPED massiv_over_1[operationsCount] = {};
           OVERLAPPED massiv_over_2[operationsCount]= {};
           
           do{
                callback = 0;
                Count_Op = 0;
                for(int i = 0; i < operationsCount ; i++){
                    massiv_over_1[i].Offset = Copy_Size_Data.LowPart;
                    massiv_over_1[i].OffsetHigh = Copy_Size_Data.HighPart;
                    Status = ReadFileEx(hfile_1, buf+i*size_copy,size_copy, &massiv_over_1[i], FileIOCompletionRoutine);
                    if(Status == 0) cout << GetLastError() << endl;
                    Copy_Size_Data.QuadPart += size_copy;
                    Count_Op ++;
                    if (Copy_Size_Data.QuadPart == Data_size.QuadPart) break;
                    
                }
                while(callback < Count_Op) SleepEx(INFINITE, TRUE);
                
                callback = 0;
                for(int i = 0; i < Count_Op; i++){
                    massiv_over_2[i].Offset = Size_Write_Data.LowPart;
                    massiv_over_2[i].OffsetHigh = Size_Write_Data.HighPart;
                    Size_Write_Data.QuadPart += size_copy;
                    Status = WriteFileEx(hfile_2, buf+i*size_copy, size_copy, &massiv_over_2[i], FileIOCompletionRoutine);
                    if(Status == 0) cout << GetLastError() << endl;
                }
                while(callback < Count_Op) SleepEx(INFINITE, TRUE);
                cout << "Data:" << Copy_Size_Data.QuadPart << endl;
                
           }while(Copy_Size_Data.QuadPart != Data_size.QuadPart);
           SetFilePointer(hfile_2,fileSizeLow,(PLONG)&fileSizeHigh, FILE_BEGIN); 
           SetEndOfFile(hfile_2);
           cout << "File Coppied" << endl;
           delete [] buf;
}

int main()
{  
    int size_copy = 0;
    int operationsCount;
    DWORD before_time;
    DWORD after_time;
    HANDLE file_1, file_2;
    file_1 = CreateFile_1();
    if(file_1 == NULL) return 0;
    file_2 = CreateFile_2();
    if(file_2 == NULL) return 0;
    
    cout << "Size to copy:" <<  endl;

    do{
        cin >> size_copy;
    }while(size_copy == 0);
    
    cout << "Count of operations:" << endl;
    do{
        cin >> operationsCount;
    }while(operationsCount == 0);
    
    before_time = GetTickCount();
    CopyFile(file_1, file_2, size_copy, operationsCount);
    after_time = GetTickCount();
    CloseHandle(file_1);
    CloseHandle(file_2);
    
    cout << "Time Operation: " << after_time-before_time << endl;
    return 0;
}