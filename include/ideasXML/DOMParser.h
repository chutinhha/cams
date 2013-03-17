// DOMParser.h: interface for the CDOMParser class.
//

#ifndef __DOMPARSER_H
#define __DOMPARSER_H
#if defined(WIN32) && !defined(__CYGWIN__)
//#include <afxtempl.h>

#if defined(DOM_MSXML4)
#if _MFC_VER < 0x0700
#include <msxml2.h>
#endif
#import <msxml4.dll>
#define MSXMLNS MSXML2
#elif defined(DOM_MSXML3)
#import <msxml3.dll>
#define MSXMLNS MSXML2
#else
#import <msxml.dll>
#define MSXMLNS MSXML
#endif

class CDOMParser
{
public:
#if defined(DOM_MSXML3) || defined(DOM_MSXML4)
	MSXMLNS::IXMLDOMDocument2Ptr m_pDOMDoc;
#else
	MSXMLNS::IXMLDOMDocumentPtr m_pDOMDoc;
#endif
	CDOMParser();
	CDOMParser( LPCTSTR szDoc );
	virtual ~CDOMParser();

	// Navigate
	BOOL Load( LPCTSTR szFileName );
	BOOL SetDoc( LPCTSTR szDoc );
	BOOL FindElem( LPCTSTR szName=NULL );
	BOOL FindChildElem( LPCTSTR szName=NULL );
	BOOL IntoElem();
	BOOL OutOfElem();
	void ResetChildPos() { if ( m_pChild ) m_pChild.Release(); };
	void ResetMainPos() { ResetChildPos(); if ( m_pMain ) m_pMain.Release(); };
	void ResetPos() { ResetMainPos(); m_pParent = m_pDOMDoc; };
	CString GetTagName() const { return x_GetTagName(m_pMain); };
	CString GetChildTagName() const { return x_GetTagName(m_pChild); };
	CString GetData() const { return x_GetData( m_pMain ); };
	CString GetChildData() const { return x_GetData(m_pChild); };
	CString GetAttrib( LPCTSTR szAttrib ) const { return x_GetAttrib( m_pMain, szAttrib ); };
	CString GetChildAttrib( LPCTSTR szAttrib ) const { return x_GetAttrib( m_pChild, szAttrib ); };
	CString GetAttribName( int n ) const;
	CString GetError() const { return m_strError; };

	// Create
	BOOL Save( LPCTSTR szFileName );
	CString GetDoc() const;
	BOOL AddElem( LPCTSTR szName, LPCTSTR szData=NULL ) { return x_AddElem(szName,szData,false,false); };
	BOOL InsertElem( LPCTSTR szName, LPCTSTR szData=NULL ) { return x_AddElem(szName,szData,true,false); };
	BOOL AddChildElem( LPCTSTR szName, LPCTSTR szData=NULL ) { return x_AddElem(szName,szData,false,true); };
	BOOL InsertChildElem( LPCTSTR szName, LPCTSTR szData=NULL ) { return x_AddElem(szName,szData,true,true); };
	BOOL AddElem( LPCTSTR szName, int nValue ) { return x_AddElem(szName,nValue,false,false); };
	BOOL InsertElem( LPCTSTR szName, int nValue ) { return x_AddElem(szName,nValue,true,false); };
	BOOL AddChildElem( LPCTSTR szName, int nValue ) { return x_AddElem(szName,nValue,false,true); };
	BOOL InsertChildElem( LPCTSTR szName, int nValue ) { return x_AddElem(szName,nValue,true,true); };
	BOOL AddAttrib( LPCTSTR szAttrib, LPCTSTR szValue ) { return x_SetAttrib(m_pMain,szAttrib,szValue); };
	BOOL AddChildAttrib( LPCTSTR szAttrib, LPCTSTR szValue ) { return x_SetAttrib(m_pChild,szAttrib,szValue); };
	BOOL AddAttrib( LPCTSTR szAttrib, int nValue ) { return x_SetAttrib(m_pMain,szAttrib,nValue); };
	BOOL AddChildAttrib( LPCTSTR szAttrib, int nValue ) { return x_SetAttrib(m_pChild,szAttrib,nValue); };
	BOOL AddSubDoc( LPCTSTR szSubDoc ) { return x_AddSubDoc(szSubDoc,false,false); };
	BOOL InsertSubDoc( LPCTSTR szSubDoc ) { return x_AddSubDoc(szSubDoc,true,false); };
	CString GetSubDoc() const { return x_GetSubDoc(m_pMain); };
	BOOL AddChildSubDoc( LPCTSTR szSubDoc ) { return x_AddSubDoc(szSubDoc,false,true); };
	BOOL InsertChildSubDoc( LPCTSTR szSubDoc ) { return x_AddSubDoc(szSubDoc,true,true); };
	CString GetChildSubDoc() const { return x_GetSubDoc(m_pChild); };
	void SetDefaultNamespace( LPCTSTR szNamespace ) { m_strDefaultNamespace = szNamespace?szNamespace:_T(""); };

	// Modify
	BOOL RemoveElem();
	BOOL RemoveChildElem();
	BOOL SetAttrib( LPCTSTR szAttrib, LPCTSTR szValue ) { return x_SetAttrib(m_pMain,szAttrib,szValue); };
	BOOL SetChildAttrib( LPCTSTR szAttrib, LPCTSTR szValue ) { return x_SetAttrib(m_pChild,szAttrib,szValue); };
	BOOL SetAttrib( LPCTSTR szAttrib, int nValue ) { return x_SetAttrib(m_pMain,szAttrib,nValue); };
	BOOL SetChildAttrib( LPCTSTR szAttrib, int nValue ) { return x_SetAttrib(m_pChild,szAttrib,nValue); };
	BOOL SetData( LPCTSTR szData, int nCDATA=0 ) { return x_SetData(m_pMain,szData,nCDATA); };
	BOOL SetChildData( LPCTSTR szData, int nCDATA=0 ) { return x_SetData(m_pChild,szData,nCDATA); };

protected:
	MSXMLNS::IXMLDOMNodePtr m_pParent;
	MSXMLNS::IXMLDOMNodePtr m_pMain;
	MSXMLNS::IXMLDOMNodePtr m_pChild;
	CString m_strError;
	CString m_strDefaultNamespace;

	HRESULT x_CreateInstance();
	BOOL x_ParseError();
	MSXMLNS::IXMLDOMNodePtr x_FindElem( MSXMLNS::IXMLDOMNodePtr pParent, MSXMLNS::IXMLDOMNodePtr pNode, LPCTSTR szPath );
	CString x_GetTagName( MSXMLNS::IXMLDOMNodePtr pNode ) const;
	CString x_GetData( MSXMLNS::IXMLDOMNodePtr pNode ) const;
	CString x_GetAttrib( MSXMLNS::IXMLDOMNodePtr pNode, LPCTSTR szAttrib ) const;
	void CDOMParser::x_Insert( MSXMLNS::IXMLDOMNodePtr pParent, MSXMLNS::IXMLDOMNodePtr pNext, MSXMLNS::IXMLDOMNodePtr pNew );
	BOOL x_AddElem( LPCTSTR szName, LPCTSTR szData, BOOL bInsert, BOOL bAddChild );
	BOOL x_AddElem( LPCTSTR szName, int nValue, BOOL bInsert, BOOL bAddChild );
	CString x_GetSubDoc( MSXMLNS::IXMLDOMNodePtr pNode ) const;
	BOOL x_AddSubDoc( LPCTSTR szSubDoc, BOOL bInsert, BOOL bAddChild );
	BOOL x_SetAttrib( MSXMLNS::IXMLDOMNodePtr pNode, LPCTSTR szAttrib, LPCTSTR szValue );
	BOOL x_SetAttrib( MSXMLNS::IXMLDOMNodePtr pNode, LPCTSTR szAttrib, int nValue );
	BOOL x_SetData( MSXMLNS::IXMLDOMNodePtr& pNode, LPCTSTR szData, int nCDATA );
};

#endif // __DOMPARSER_H
#endif