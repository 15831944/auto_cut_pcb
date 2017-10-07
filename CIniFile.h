/*==================================================================
=  �ļ���:CIniFile�ඨ���ļ�                                       =  
=  ��Ҫ����:���Զ�ȡ.�޸ı�����ֵ,���������µ���,�µı���          =
            =                            = 
====================================================================*/
#ifndef _CINIFILE_H_
#define _CINIFILE_H_
//�û��ӿ�˵��:�ڳ�Ա����SetVarStr��SetVarInt������,��iType������,������û��ƶ��Ĳ�����ini�ļ��в�����,
//���д���µı���.��iType��������,������û��ƶ��Ĳ�����ini�ļ��в�����,�Ͳ�д���µı���������ֱ�ӷ���FALSE;
class CIniFile
{
public:
	CIniFile();
	virtual ~CIniFile();
private:
	CIniFile(const CIniFile &);
	CIniFile & operator = (const CIniFile &);
public:
	//��������
	BOOL Create(const CString &strFileName);
	//�õ�������������ֵ
	BOOL GetVarInt(const CString &,const CString & ,int &);
	BOOL GetVarShort(const CString &,const CString &, short &);
	//�õ������ַ�������ֵ
	BOOL GetVarStr(const CString &,const CString & ,CString & );
	BOOL GetVarFloat(const CString & strSection,const CString & strVarName,double & fValue);
		
	//�������ñ�����������ֵ
	BOOL SetVarInt(const CString &,const CString & ,const int &,const int iType = 1);
	//�������ñ����ַ�������ֵ
	BOOL SetVarStr(const CString &,const CString &, const CString &,const int iType = 1);
   //�������ø�������ֵ
	BOOL SetVarFloat(const CString & strSection,const CString & strVarName,const double & fValue,const int iType=1);
		
	
private:
	BOOL GetVar(const CString &,const CString &,CString &);
	BOOL SetVar(const CString &,const CString &,const CString &,const int iType = 1);
	int  SearchLine(const CString &,const CString &);

private:
//	vector <CString>  FileContainer;
	CArray <CString,CString> FileContainer;
	BOOL  bFileExsit;
	CStdioFile stfFile;
	CString strInIFileName;
	
};

#endif