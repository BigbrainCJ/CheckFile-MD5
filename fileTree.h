#pragma once
#include "sta.h"
using namespace std;
typedef struct fileTree {
	bool flag;   //0Ϊ�ļ��У�1Ϊ�ļ�
	string fileName;
	string vue_md5;
	fileTree* sun;
	fileTree* brother;
}fileTree;

fileTree* nNode(bool flag, string fileName, string vue_md5)
{
	fileTree* newNode = new fileTree;
	newNode->brother = newNode->sun = NULL;
	newNode->flag = flag;
	newNode->fileName = fileName;

	//���Ϊ�ļ��������md5ֵ
	if (flag)
		newNode->vue_md5 = vue_md5;
	return newNode;
}
void findFile(string dir, fileTree* father)
{
	fileTree* node = nNode(0, dir, "");      //�����ļ��нڵ�
	if (father->sun == NULL)
		father->sun = node;
	else
	{
		fileTree* ptr = father->sun;
		while (ptr->brother != NULL)
			ptr = ptr->brother;
		ptr->brother = node;

	}
	WIN32_FIND_DATAA stFD;							//����ļ���Ϣ�Ľṹ��
	HANDLE h;
	string temp;

	temp = dir + "\\*";
	h = FindFirstFileA(temp.c_str(), &stFD);			//����Ŀ¼���



	while (FindNextFileA(h, &stFD))						//��ȡĿ¼�����ӦĿ¼���������ļ�
	{
		temp = dir + "\\" + stFD.cFileName;

		if (temp == dir + "\\..")										//��һ��·��
		{
			continue;
		}
		else if (PathIsDirectoryA(temp.c_str()))		//������Ŀ¼
		{
			findFile(temp, node);												//�ݹ����
		}
		else
		{
			string md5 = GetMd5(stringToLPCWSTR(temp));
			fileTree* nodeSun = nNode(1, temp, md5);      //�����ļ��ڵ�
			if (node->sun == NULL)
				node->sun = nodeSun;
			else
			{
				fileTree* ptr = node->sun;
				while (ptr->brother != NULL)
					ptr = ptr->brother;
				ptr->brother = nodeSun;

			}
			//cout << stFD.cFileName << endl;				//��ӡ�ļ���
		}
	}

	return;
}

void print(fileTree* node, int deep)
{
	for (int i = 0; i < deep; i++)
		cout << "|";
	cout << node->fileName;
	if (node->flag)
		cout << "  " << "MD5:" << node->vue_md5;
	cout << endl;

	fileTree* ptr = node->sun;
	while (ptr != NULL)
	{
		print(ptr, deep + 1);
		ptr = ptr->brother;
	}
}