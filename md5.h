
#include "sta.h"
using namespace std;

LPCWSTR stringToLPCWSTR(std::string orig)
{
	size_t origsize = orig.length() + 1;
	const size_t newsize = 100;
	size_t convertedChars = 0;
	wchar_t* wcstring = (wchar_t*)malloc(sizeof(wchar_t) * (orig.length() - 1));
	mbstowcs_s(&convertedChars, wcstring, origsize, orig.c_str(), _TRUNCATE);

	return wcstring;
}
string GetMd5(LPCWSTR FileDirectory)
{
	HANDLE hFile = CreateFile(FileDirectory, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, NULL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)                                        //���CreateFile����ʧ��
	{
		cout << "CreateFile go wrong :" << GetLastError() << endl;                //��ʾCreateFile����ʧ�ܣ����������š�visual studio�п��ڡ����ߡ�>��������ҡ������ô���ŵõ�������Ϣ��
		CloseHandle(hFile);
	}
	HCRYPTPROV hProv = NULL;
	if (CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT) == FALSE)       //���CSP��һ����Կ�����ľ��
	{
		cout << "CryptAcquireContext go wrong:" << GetLastError() << endl;
	}
	HCRYPTPROV hHash = NULL;
	if (CryptCreateHash(hProv, CALG_MD5, 0, 0, &hHash) == FALSE)     //��ʼ������������hash������������һ����CSP��hash������صľ��������������������CryptHashData���á�
	{
		cout << "CryptCreateHash go wrong:" << GetLastError() << endl;
	}
	DWORD dwFileSize = GetFileSize(hFile, 0);    //��ȡ�ļ��Ĵ�С
	if (dwFileSize == 0xFFFFFFFF)               //�����ȡ�ļ���Сʧ��
	{
		cout << "GetFileSize go wrong:" << GetLastError() << endl;
	}
	byte* lpReadFileBuffer = new byte[dwFileSize];
	DWORD lpReadNumberOfBytes;
	if (ReadFile(hFile, lpReadFileBuffer, dwFileSize, &lpReadNumberOfBytes, NULL) == 0)        //��ȡ�ļ�
	{
		cout << "ReadFile go wrong:" << GetLastError() << endl;
	}
	if (CryptHashData(hHash, lpReadFileBuffer, lpReadNumberOfBytes, 0) == FALSE)      //hash�ļ�
	{
		cout << "CryptHashData go wrong:" << GetLastError() << endl;
	}

	delete[] lpReadFileBuffer;
	CloseHandle(hFile);          //�ر��ļ����
	BYTE* pbHash;
	DWORD dwHashLen = sizeof(DWORD);
	//����ע�͵��Ĵ��벻��ʹ�ã���Ϊ�Ѿ�֪��md5ֵ��ռ32���ֽڣ�û�б�Ҫͨ��CryptGetHashParam�������õ��ֽ�����
	/*
	BYTE *pbHashSize;
	if (!(pbHashSize=(byte*)malloc(dwHashLen)))      //ΪpbHashSize�����ڴ�
	{
		cout<<"memory allocation failed:"<<GetLastError()<<endl;
	}
	//���ڶ���������ֵ��ΪHP_HASHSIZE��dwHashLen�д����hashֵ���ֽ�����������ñ����ڽ���������������ΪHP_HASHVAL�ĵ���ǰ���������ܷ�����ȷ�������ڴ档
	if (CryptGetHashParam(hHash,HP_HASHSIZE,pbHashSize,&dwHashLen,0))
	{
		free(pbHashSize);
	}
	else
	{
		cout<<"get size go wrong"<<GetLastError()<<endl;
	}*/
	if (CryptGetHashParam(hHash, HP_HASHVAL, NULL, &dwHashLen, 0))      //��Ҳ��֪��ΪʲôҪ����������CryptGetHashParam������ǲ��յ�msdn       
	{
	}
	else
	{
		cout << "get length wrong" << endl;
	}
	if (pbHash = (byte*)malloc(dwHashLen))
	{
	}
	else
	{
		cout << "allocation failed" << endl;
	}
	if (CryptGetHashParam(hHash, HP_HASHVAL, pbHash, &dwHashLen, 0))            //���md5ֵ
	{
		string str = "";
		
		for (DWORD i = 0; i < dwHashLen; i++)         //���md5ֵ
		{
			if ((int)pbHash[i] / 16 < 10)
				str += (int)pbHash[i] / 16+48;
			else
				str += (int)pbHash[i] / 16 - 10 + 97;
			
			if ((int)pbHash[i] % 16 < 10)
				str += (int)pbHash[i] % 16+48;
			else
				str += (int)pbHash[i] % 16 - 10 + 97;

			//printf("%02x", pbHash[i]);	
		}
	
		return str;
	}
	//�ƺ���
	if (CryptDestroyHash(hHash) == FALSE)          //����hash����
	{
		cout << "CryptDestroyHash go wrong:" << GetLastError() << endl;
	}
	if (CryptReleaseContext(hProv, 0) == FALSE)
	{
		cout << "CryptReleaseContext go wrong:" << GetLastError() << endl;
	}
}


