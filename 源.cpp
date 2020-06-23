#include "md5.h"
#include "fileTree.h"
using namespace std;
/* 
    Ŀ¼����Ŀ¼�£�
	     �����������ļ��м��ļ���
		 �����������ļ��м��ļ���
		 �����������ļ��м��ļ���
*/

void compare(fileTree* father1,fileTree* father2)
{
	fileTree* ptr1 = father1->sun;
	fileTree* ptr2;
	while (ptr1 != NULL)
	{
		bool ifFound = 0;
		ptr2 = father2->sun;
		while (ptr2 != NULL)
		{
			if (ptr1->fileName == ptr2->fileName)
			{
				ifFound = 1;
				break;
			}
			ptr2 = ptr2->brother;
		}

		if (ifFound)
		{
			if(PathIsDirectoryA(ptr1->fileName.c_str()))//������ļ��У���ݹ�
			{
				compare(ptr1, ptr2);
			}
			else         //������ļ�����Ƚ�md5ֵ
			{
				if (ptr1->vue_md5 != ptr2->vue_md5)
					cout << "�޸ĵ㣺" << ptr1->fileName << "-----" << ptr2->fileName << endl;
				
			}
		}
		else
		{
			cout << "ɾ����:" << ptr1->fileName << endl;
		}

		ptr1 = ptr1->brother;
	}

	ptr2 = father2->sun;
	while (ptr2 != NULL)
	{
		bool ifFound = 0;
		ptr1 = father1->sun;
		while (ptr1 != NULL)
		{
			if (ptr1->fileName == ptr2->fileName)
			{
				ifFound = 1;
				break;
			}
			ptr1 = ptr1->brother;
		}

		if (!ifFound)
			cout << "���Ӵ�:" << ptr2->fileName << endl;


		ptr2 = ptr2->brother;
	}
}
void main()
{
	string filePath;
	cout << "�ļ���:" << endl;
	cin >> filePath;
	
	fileTree* root1,*root2;

	root1 = nNode(0, "root", "");
	findFile(filePath, root1);

	cout << "���������ַ��Կ�ʼ�Ƚ�..." << endl;
	int temp;
	cin >> temp;
	root2 = nNode(0, "root", "");
	findFile(filePath, root2);
	compare(root1, root2);
	
	
	system("pause");
}