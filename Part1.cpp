#include <windows.h>
#include <stdio.h>
#include <iostream>

using namespace std;
void menu()
{
    cout << "1 - Disk List" << endl;
    cout << "2 - Info disk and its free size" << endl;
    cout << "3 - Creating and deleting specified directories" << endl;
    cout << "4 - Creation in the specified directory" << endl;
    cout << "5 - Copying and moving files between directories" << endl;
    cout << "6 - File attribute analysis and modification" << endl;
    cout << "0 - Exiting the program" << endl;
}

void DiskList() //Вывод списка дисков, GetLogicalDrives
{
    int n;
    char OutputDisks[4];
    DWORD BitMask = GetLogicalDrives(); //Получаем битовую маску
    for (int i = 1; i < 26; i++) //через ASCII узнаем имя диска
    {
        n = (BitMask & (1<<i));
        if(n)
        {
            OutputDisks[0] = char(65+i); OutputDisks[1] = ':'; OutputDisks[2] = 47; OutputDisks[3] = 0;
            cout << "Aviable disk drives: " << OutputDisks << endl;
        }
    }
}

void GetDiskInformation()
{
    // Информация о диске, Getdrivetype,GetVolumeInformation
    string select_disk;
    LPCWSTR  name;
    UINT x;
    cout << "Select a disk (path)" << endl;
    cin >> select_disk;
    name =  (LPCWSTR) (select_disk.c_str());
    x = GetDriveType((LPCSTR) (name));
    if (x == 0) cout << "Disk type is not determined." << endl; //Тип диска определить невозможно
    if (x == 1) cout << "Root is not exists." << endl; //Корня не существует
    if (x == DRIVE_UNKNOWN) cout << "The drive type cannot be determined." << endl; // Тип диска определить невозможно.
    if (x == DRIVE_NO_ROOT_DIR) cout << "The root path is invalid; for example, there is no volume mounted at the specified path." << endl; // Корневой путь недопустим; например, по указанному пути не установлен том.
    if (x == DRIVE_REMOVABLE) cout << "The drive has removable media; for example, a floppy drive, thumb drive, or flash card reader." << endl; // На диске есть съемные носители; например, дисковод гибких дисков, флэш-накопитель или устройство для чтения флэш-карт.
    if (x == DRIVE_FIXED) cout << "The drive has fixed media; for example, a hard disk drive or flash drive." << endl; // На диске есть фиксированные носители; например, жесткий диск или флэш-накопитель.
    if (x == DRIVE_REMOTE) cout << "The drive is a remote (network) drive." << endl; // Диск является удаленным (сетевым) диском.
    if (x == DRIVE_CDROM) cout << "The drive is a CD-ROM drive." << endl; // Привод представляет собой дисковод для компакт-дисков.
    if (x == DRIVE_RAMDISK) cout << "The drive is a RAM disk." << endl; // Накопитель представляет собой ОЗУ.

    BOOL GetDiskFreeSpaceFlag;
    BOOL GetVolumeInformationFlag;
    char lpVolumeNameBuffer[100];        // название диска                  [out]
    DWORD nVolumeNameSize = 100;            // длина буфера названия диска     [in] 
    unsigned long lpVolumeSerialNumber;     // сериальный номер диска          [out]
    LPDWORD lpMaximumComponentLength = NULL; // максимальная длина фыйла        [out]
    LPDWORD lpFileSystemFlags = NULL;        // опции файловой системы          [out]
    char lpFileSystemNameBuffer[100];    // имя файловой системы            [out]
    DWORD nFileSystemNameSize = 100;         // длина буфера имени файл. сист.  [in]

    u_int64 FreeBytesAvailable;    // доступно для использования(байт) [out]
    u_int64 TotalNumberOfBytes;    // максимальный объём( в байтах )   [out]
    u_int64 TotalNumberOfFreeBytes; // свободно на диске( в байтах )    [out]

    GetVolumeInformationFlag = GetVolumeInformationA(
    (LPCSTR) (name),
    lpVolumeNameBuffer,
    nVolumeNameSize,
    &lpVolumeSerialNumber,
    lpMaximumComponentLength,
    lpFileSystemFlags,
    lpFileSystemNameBuffer,
    nFileSystemNameSize
    );
    if(GetVolumeInformationFlag != 0) 
    {
        cout << "Volume Serial Number is " << lpVolumeSerialNumber << endl;
        cout << "File System is " << lpFileSystemNameBuffer << endl;
    }
    else cout << "Not Present (GetVolumeInformation)" << endl;

    GetDiskFreeSpaceFlag = GetDiskFreeSpaceEx(
    (LPCSTR) (name), // directory name
    (PULARGE_INTEGER)&FreeBytesAvailable,     // bytes available to caller
    (PULARGE_INTEGER)&TotalNumberOfBytes,     // bytes on disk
    (PULARGE_INTEGER)&TotalNumberOfFreeBytes  // free bytes on disk
    );
    
    if(GetDiskFreeSpaceFlag != 0) 
    {
        cout << "Total Number Of Free Bytes = " << TotalNumberOfFreeBytes 
         << "(" <<  ((long double)((TotalNumberOfFreeBytes)))/1024/1024/1024
         << " Gb)" << endl;
        cout << "Total Number Of Bytes = " << TotalNumberOfBytes 
         << " (" << ((long double)((TotalNumberOfBytes)))/1024/1024/1024
         << " Gb)" << endl;
    }
    else cout << "Not Present (GetDiskFreeSpace)" << endl;
}

void CreateDeleteDir()
{
 int choise;
    string select_disk;
    LPCWSTR  name;
    int dirinput;
    cout << "1 - Create Directory" << endl;
    cout << "2 - Delete Directory" << endl;       
    cin >> dirinput;
    switch(dirinput){
        case 1:
            cout << "Enter path" << endl;
            cin >> select_disk;
            name =  (LPCWSTR) (select_disk.c_str());
            if (CreateDirectory((LPCSTR) name,NULL))
                cout << "Directory created" << endl;
            else
                cout << "Error" << endl;
            break;
        case 2:
            cout << "Enter path" << endl;
            cin >> select_disk;
            name =  (LPCWSTR) (select_disk.c_str());
            if (RemoveDirectory((LPCSTR) name))
                cout << "Directory deleted" << endl;
            else
                cout << "Error" << endl;
            break;
        default:
            break;
    }
}

void CreateFile()
{
    LPCWSTR  name;
    string select_file;
    cout << "Select a file path and name" << endl;
    cin >> select_file;
    name =  (LPCWSTR) (select_file.c_str());
    HANDLE hfile = CreateFile((LPCSTR) name,  GENERIC_WRITE | GENERIC_READ, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hfile != INVALID_HANDLE_VALUE)
        cout << "File created" << endl;
    else
        cout << "Error" << endl;
}

void Copyfile()
{
    int copymoveinput;
    LPCWSTR  name1;
    LPCWSTR  name2;
    string select_file1;
    string select_file2;
    WINBOOL hfile;
    BOOL gfile;

    cout << "Copy or Move your file" << endl;
    cout << "1 - Copy" << endl;
    cout << "2 - Move" << endl;
    cin >> copymoveinput;
       switch(copymoveinput){
        case 1:
            cout << "Select the file to copy" << endl;
            cin >> select_file1;
            cout << "Select the copy path" << endl;
            cin >> select_file2;
            name1 =  (LPCWSTR) (select_file1.c_str());
            name2 =  (LPCWSTR) (select_file2.c_str());
            hfile = CopyFile((LPCSTR) name1,(LPCSTR) name2, 1);
            if(hfile  == 0) cout << "Already created, error" << endl;
            else cout << "Done" << endl;
            break;
        case 2:
            cout << "Select the file to move" << endl;
            cin >> select_file1;
            cout << "Select the directory to move file" << endl;
            cin >> select_file2;
            name1 =  (LPCWSTR) (select_file1.c_str());
            name2 =  (LPCWSTR) (select_file2.c_str());
            gfile = MoveFileEx((LPCTSTR) name1,(LPCTSTR) name2, MOVEFILE_COPY_ALLOWED | MOVEFILE_FAIL_IF_NOT_TRACKABLE | MOVEFILE_REPLACE_EXISTING);
            if(gfile == 0 ) cout << "Error" << endl;
            else cout << "Done" << endl;
            break;
        default:
            break;
    }
    
}

void PrintFileTime(FILETIME time){
    SYSTEMTIME time_1;
    if(FileTimeToSystemTime(&time, &time_1)){
        cout << "Year: " << time_1.wYear <<endl;
        cout << "Month: " << time_1.wMonth <<endl;
        cout << "Day: " << time_1.wDay <<endl;
        cout << "Hour: " << time_1.wHour <<endl;
        cout << "Minutes: " << time_1.wMinute <<endl;
        cout << "Seconds: " << time_1.wSecond <<endl;
    }
    else cout << "Error" << endl;
}

void SetNewFileTime(HANDLE hFile){
    SYSTEMTIME time_1;
    FILETIME time;
    int Year;
    int Month;
    int Day;
    int Hour;
    int Minutes;
    int Seconds;
    
    cout << "Input Year" << endl;
    cin >> Year;
    cout << "Input Month" << endl;
    cin >> Month;
    cout << "Input Day" << endl;
    cin >> Day;
    cout << "Input Hour" << endl;
    cin >> Hour;
    cout << "Input Minutes" << endl;
    cin >> Minutes;
    cout << "Input Seconds" << endl;
    cin >> Seconds;
    time_1.wYear = Year;
    time_1.wMonth = Month;
    time_1.wDay = Day;
    time_1.wHour = Hour;
    time_1.wMinute = Minutes;
    time_1.wSecond = Seconds;
    if(SystemTimeToFileTime(&time_1, &time)){
        SetFileTime(hFile, &time, &time, &time);
    }
    else cout << "Error" << endl;
}

void InfoAtributs(){
    string select_file;
    wstring w_string;
    LPCWSTR  name;
    DWORD atributs;
    int choise;
    int input;
    int n;
    HANDLE hFile;
    BY_HANDLE_FILE_INFORMATION fhi;
    BOOL fResult;
    
    FILETIME lpCreationTime;
    FILETIME lpLastAccessTime;
    FILETIME lpLastWriteTime;
    
    cout << "1 - Info Atributes File; 2 - Change file Atributes, 3 - File Information, 0 - Exit Program)" << endl;
    cin >> choise;
    switch(choise){
        case 1:
            cout << "Select a file:" << endl;
            cin >> select_file;
            name =  (LPCWSTR) (select_file.c_str());
            atributs = GetFileAttributes((LPCSTR) name);
            if(atributs & FILE_ATTRIBUTE_ARCHIVE ) cout << "Archive" << endl; // Архивный
            if(atributs & FILE_ATTRIBUTE_COMPRESSED) cout << "Compressed" << endl; // Сжатый
            if(atributs & FILE_ATTRIBUTE_DIRECTORY) cout <<"Name Directory" << endl; // Имя папки
            if(atributs & FILE_ATTRIBUTE_HIDDEN) cout <<"Hiden Files or Directory" << endl; // Скрытый файл или директория
            if(atributs & FILE_ATTRIBUTE_NORMAL) cout <<"File no Atributes" << endl; // Файл не имеет атрибутов
            if(atributs & FILE_ATTRIBUTE_READONLY) cout <<"File Read" << endl; // Файл только для чтения
            if(atributs & FILE_ATTRIBUTE_SYSTEM) cout <<"System File" << endl; // Системный файл
            break;
        case 2:
            cout << "Select a file" << endl;
            cin >> select_file;
            name =  (LPCWSTR) (select_file.c_str());
            cout << "Choose which attribute you want to replace the file with(1 - Archive, 2 - Hiden Files or Directory, 3 - File Read, 4 - Compressed, 5 - Name Directory, 6 - System File)" << endl;
            cin >> input;
            switch(input){
                    case 1:
                        SetFileAttributes( (LPCSTR) name, FILE_ATTRIBUTE_ARCHIVE);
                        atributs = GetFileAttributes((LPCSTR) name);
                        if( atributs & FILE_ATTRIBUTE_ARCHIVE) cout << "Done" << endl;
                        else cout << "Error" << endl;
                        break;
                    case 2:
                        SetFileAttributes( (LPCSTR) name, FILE_ATTRIBUTE_HIDDEN);
                        atributs = GetFileAttributes((LPCSTR) name);
                        if( atributs & FILE_ATTRIBUTE_HIDDEN) cout << "Done" << endl;
                        else cout << "Error" << endl;
                        break;
                    case 3:
                        SetFileAttributes( (LPCSTR) name, FILE_ATTRIBUTE_READONLY);
                        atributs = GetFileAttributes((LPCSTR) name);
                        if( atributs & FILE_ATTRIBUTE_READONLY) cout << "Done" << endl;
                        else cout << "Error" << endl;
                        break;
                    case 4:
                        SetFileAttributes( (LPCSTR) name, FILE_ATTRIBUTE_COMPRESSED);
                        atributs = GetFileAttributes((LPCSTR) name);
                        if( atributs & FILE_ATTRIBUTE_COMPRESSED) cout << "Done" << endl;
                        else cout << "Error" << endl;
                        break;
                    case 5:
                        SetFileAttributes( (LPCSTR) name, FILE_ATTRIBUTE_DIRECTORY);
                        atributs = GetFileAttributes((LPCSTR) name);
                        if( atributs & FILE_ATTRIBUTE_DIRECTORY) cout << "Done" << endl;
                        else cout << "Error" << endl;
                        break;
                    case 6:
                        SetFileAttributes( (LPCSTR) name, FILE_ATTRIBUTE_SYSTEM);
                        atributs = GetFileAttributes((LPCSTR) name);
                        if( atributs & FILE_ATTRIBUTE_SYSTEM) cout << "Done" << endl;
                        else cout << "Error" << endl;
                        break;
                    default:
                        break;
                }
            break;
        case 3:
            cout << "Select the file" << endl;
            cin >> select_file;
            name =  (LPCWSTR) (select_file.c_str());
            hFile = CreateFile( (LPCSTR) name, GENERIC_READ | GENERIC_WRITE | DELETE, 0, NULL, CREATE_ALWAYS, 0, NULL);
            if (hFile != INVALID_HANDLE_VALUE){
                fResult = GetFileInformationByHandle( hFile, &fhi);
                if (fResult){
                    cout << "GetFileInformationByHandle got file information" << endl; // Получена информация о файле
                    cout << "Atributes: " << fhi.dwFileAttributes << endl;
                    if(GetFileTime(hFile, &lpCreationTime, &lpLastAccessTime, &lpLastWriteTime)){
                        cout << "Creation Time" << endl;
                        PrintFileTime(lpCreationTime);
                        cout << "Acess Time" << endl;
                        PrintFileTime(lpLastAccessTime);
                        cout << "Last Write Time" << endl;
                        PrintFileTime(lpLastWriteTime);
                    }
                    cout << "Serial Number: " << fhi.dwVolumeSerialNumber <<endl;
                    cout << "File Size(High): " << fhi.nFileSizeHigh <<endl;
                    cout << "File Size(Low): " << fhi.nFileSizeLow <<endl;
                    cout << "Number of Links: " << fhi.nNumberOfLinks <<endl;
                    cout << "Index(High): " << fhi.nFileIndexHigh <<endl;
                    cout << "Index(Low): " << fhi.nFileIndexLow <<endl;
                    
                    do{
                        cout << "Would you like to change the file time?(1 - Yes, 2 - No)" << endl;
                        cin >> n;
                        if(n == 1){
                            SetNewFileTime(hFile);
                        }
                        else cout << " Please Select 2" << endl;
                    }while(n != 2);
                }
                else cout << "Error" << endl;
                
                CloseHandle(hFile);
            }
            break;
        default:
            break;    
    }
    
}

int main()
{
    int input;
    do
    {
        system("cls");
        menu();
        cin >> input;
        switch (input) {
        case 1:
            DiskList(); 
            break;
        case 2:
            GetDiskInformation();    
            break;
        case 3:
            CreateDeleteDir();
            break;
        case 4:
            CreateFile();
            break;
        case 5:
            Copyfile();
            break;
        case 6:
            InfoAtributs();
            break;
        case 0:
            cout << "Exit Program" << endl;
            break;
        default:
            break;
        }
        system("pause");
    }while (input != 0);
    return 0;
}