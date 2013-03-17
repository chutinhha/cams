// DOMParser.cpp: implementation of the CDOMParser class.
//
#if defined(WIN32) && !defined(__CYGWIN__)
//#include "IdGlobals.h"
#include "IdCommons.h"
#include "DOMParser.h"

CDOMParser::CDOMParser()
{
	CoInitialize(NULL);
	x_CreateInstance();
}

CDOMParser::CDOMParser( LPCTSTR szDoc )
{
	CoInitialize(NULL);
	if ( SUCCEEDED(x_CreateInstance()) )
		SetDoc( szDoc );
}

CDOMParser::~CDOMParser()
{
	// Release COM interfaces, in case CoUninitialize is being called the last time
	if ( m_pChild )
		m_pChild.Release();
	if ( m_pMain )
		m_pMain.Release();
	if ( m_pParent )
		m_pParent.Release();
	if ( m_pDOMDoc )
		m_pDOMDoc.Release();
	CoUninitialize();
}

HRESULT CDOMParser::x_CreateInstance()
{
	// Release any reference to a previous instance
	if ( m_pParent )
		m_pParent.Release();
	if ( m_pDOMDoc )
		m_pDOMDoc.Release();

	// Create new instance
#if defined( MARKUP_MSXML4 )
	HRESULT hr = m_pDOMDoc.CreateInstance( __uuidof(MSXML2::DOMDocument40) );
#elif defined( MARKUP_MSXML3 )
	HRESULT hr = m_pDOMDoc.CreateInstance( __uuidof(MSXML2::DOMDocument) );
#else
	HRESULT hr = m_pDOMDoc.CreateInstance( "Microsoft.XMLDOM" );
#endif
	if ( FAILED(hr) )
	{
		if ( hr == REGDB_E_CLASSNOTREG )
			printf("MSXML not registered");
			//AfxMessageBox( _T("MSXML not registered") );
		else
			printf("Unable to create MSXML instance");
			//AfxMessageBox( _T("Unable to create MSXML instance") );
	}
	// m_pDOMDoc->PutpreserveWhiteSpace( TRUE );
	m_pParent = m_pDOMDoc;
	return hr;
}

BOOL CDOMParser::x_ParseError()
{
	MSXMLNS::IXMLDOMParseErrorPtr pDOMParseError;
	m_pDOMDoc->get_parseError( &pDOMParseError );
	m_strError = (LPCTSTR)pDOMParseError->Getreason();
	m_strError.TrimRight( _T("\r\n") );
	return FALSE;
}

BOOL CDOMParser::SetDoc( LPCTSTR szDoc )
{
	ResetPos();

	// If szDoc is empty, clear it
	if ( ! szDoc || ! szDoc[0] )
		return SUCCEEDED( x_CreateInstance() );

#ifdef _UNICODE
	_bstr_t bstrDoc(szDoc);
	VARIANT_BOOL bResult = m_pDOMDoc->loadXML( bstrDoc );
#else
	USES_CONVERSION;
	_bstr_t bstrDoc(A2BSTR(szDoc),false);
	VARIANT_BOOL bResult = m_pDOMDoc->loadXML( bstrDoc );
#endif
	if ( ! bResult )
		return x_ParseError();
	return TRUE;
};

BOOL CDOMParser::Load( LPCTSTR szFileName )
{
	_variant_t varName;
#ifdef _UNICODE
	varName.vt = VT_BSTR;
	varName.bstrVal = SysAllocString(szFileName);
#else
	varName.SetString(szFileName);
#endif
	VARIANT_BOOL bResult = m_pDOMDoc->load( varName );
	ResetPos();
	if ( ! bResult )
		return x_ParseError();
	return TRUE;
}


BOOL CDOMParser::Save( LPCTSTR szFileName )
{
	_variant_t varName;
#ifdef _UNICODE
	varName.vt = VT_BSTR;
	varName.bstrVal = SysAllocString(szFileName);
#else
	varName.SetString(szFileName);
#endif
	HRESULT hr = m_pDOMDoc->save( varName );
	if ( hr )
		return FALSE;
	return TRUE;
}

CString CDOMParser::GetDoc() const
{
	return (LPCTSTR)m_pDOMDoc->xml;
};

BOOL CDOMParser::FindElem( LPCTSTR szName )
{
	// Change current position only if found
	//
	MSXMLNS::IXMLDOMNodePtr pNode;
	pNode = x_FindElem( m_pParent, m_pMain, szName );
	if ( pNode )
	{
		m_pMain = pNode;
		m_pParent = m_pMain->GetparentNode();
		if ( m_pChild )
			m_pChild.Release();
		return TRUE;
	}
	return FALSE;
}

BOOL CDOMParser::FindChildElem( LPCTSTR szName )
{
	// Change current child position only if found
	//
	// Shorthand: call this with no current main position
	// means find child under root element
	if ( ! ((bool)(m_pParent->GetparentNode())) && ! ((bool)m_pMain) )
		FindElem();
	if ( ! ((bool)m_pMain) )
		return FALSE;

	MSXMLNS::IXMLDOMNodePtr pNode;
	pNode = x_FindElem( m_pMain, m_pChild, szName );
	if ( pNode )
	{
		m_pChild = pNode;
		m_pMain = m_pChild->GetparentNode();
		m_pParent = m_pMain->GetparentNode();
		return TRUE;
	}

	return FALSE;
}

BOOL CDOMParser::IntoElem()
{
	if ( m_pMain )
	{
		m_pParent = m_pMain;
		if ( m_pChild )
		{
			m_pMain = m_pChild;
			m_pChild.Release();
		}
		else
			m_pMain.Release();
		return TRUE;
	}
	return FALSE;
}

BOOL CDOMParser::OutOfElem()
{
	if ( (bool)(m_pParent->GetparentNode()) )
	{
		m_pChild = m_pMain;
		m_pMain = m_pParent;
		m_pParent = m_pMain->GetparentNode();
		return TRUE;
	}
	return FALSE;
}

CString CDOMParser::GetAttribName( int n ) const
{
	CString strAttribName;
	if ( ! (bool)m_pMain )
		return strAttribName;

	// Is it within range?
	if ( n >= 0 && n < m_pMain->Getattributes()->Getlength() )
	{
		MSXMLNS::IXMLDOMNodePtr pAttrib = m_pMain->Getattributes()->item[n];
		strAttribName = (LPCTSTR)pAttrib->GetnodeName();
	}
	return strAttribName;
}

BOOL CDOMParser::RemoveElem()
{
	if ( m_pMain )
	{
		MSXMLNS::IXMLDOMNodePtr pParent = m_pMain->GetparentNode();
		if ( pParent )
		{
			if ( m_pChild )
				m_pChild.Release();
			MSXMLNS::IXMLDOMNodePtr pPrev = m_pMain->GetpreviousSibling();
			pParent->removeChild( m_pMain );
			m_pMain = pPrev;
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CDOMParser::RemoveChildElem()
{
	if ( m_pChild )
	{
		MSXMLNS::IXMLDOMNodePtr pPrev = m_pChild->GetpreviousSibling();
		m_pMain->removeChild( m_pChild );
		m_pChild.Release();
		m_pChild = pPrev;
		return TRUE;
	}
	return FALSE;
}


//////////////////////////////////////////////////////////////////////
// Private Methods
//////////////////////////////////////////////////////////////////////

MSXMLNS::IXMLDOMNodePtr CDOMParser::x_FindElem( MSXMLNS::IXMLDOMNodePtr pParent, MSXMLNS::IXMLDOMNodePtr pNode, LPCTSTR szPath )
{
	// If szPath is NULL or empty, go to next sibling element
	// Otherwise go to next sibling element with matching path
	//
	if ( pNode )
		pNode = pNode->GetnextSibling();
	else
		pNode = pParent->GetfirstChild();

	while ( pNode )
	{
		if ( pNode->nodeType == MSXMLNS::NODE_ELEMENT )
		{
			// Compare tag name unless szPath is not specified
			if ( szPath == NULL || !szPath[0] || x_GetTagName(pNode) == szPath )
				break;
		}
		pNode = pNode->GetnextSibling();
	}
	return pNode;

}

CString CDOMParser::x_GetTagName( MSXMLNS::IXMLDOMNodePtr pNode ) const
{
	CString strTagName = (LPCTSTR)pNode->GetnodeName();
	return strTagName;
}

CString CDOMParser::x_GetAttrib( MSXMLNS::IXMLDOMNodePtr pNode, LPCTSTR szAttrib ) const
{
	CString strAttrib;
	MSXMLNS::IXMLDOMNodePtr pAttrib;
	HRESULT hr = pNode->Getattributes()->raw_getNamedItem( _bstr_t(szAttrib), &pAttrib );
	if ( SUCCEEDED(hr) && ((bool)pAttrib) )
	{
		_variant_t varVal = pAttrib->GetnodeValue();
		if ( varVal.vt == VT_BSTR )
			strAttrib = (LPCTSTR)_bstr_t(varVal.bstrVal);
	}
	return strAttrib;
}

void CDOMParser::x_Insert( MSXMLNS::IXMLDOMNodePtr pParent, MSXMLNS::IXMLDOMNodePtr pNext, MSXMLNS::IXMLDOMNodePtr pNew )
{
	if ( pNext )
	{
		VARIANT varRef;
		VariantInit( &varRef );
		varRef.vt = VT_DISPATCH;
		varRef.pdispVal = pNext.GetInterfacePtr();
		pParent->insertBefore( pNew, varRef );
	}
	else
		pParent->appendChild( pNew );
}

BOOL CDOMParser::x_AddElem( LPCTSTR szName, int nValue, BOOL bInsert, BOOL bAddChild )
{
	// Convert integer to string
	_TCHAR szVal[25];
	_stprintf( szVal, _T("%d"), nValue );
	return x_AddElem( szName, szVal, bInsert, bAddChild );
}

BOOL CDOMParser::x_AddElem( LPCTSTR szName, LPCTSTR szData, BOOL bInsert, BOOL bAddChild )
{
	MSXMLNS::IXMLDOMNodePtr pNext, pParent;
	if ( bAddChild )
	{
		if ( ! (bool)m_pMain )
			return FALSE;
		pParent = m_pMain;
		pNext = m_pChild;
	}
	else
	{
		if ( m_pChild )
			m_pChild.Release();
		pParent = m_pParent;
		pNext = m_pMain;
	}
	if ( bInsert )
	{
		if ( ! ((bool)pNext) )
			pNext = pParent->GetfirstChild();
	}
	else
	{
		if ( pNext )
			pNext = pNext->GetnextSibling();
	}

	MSXMLNS::IXMLDOMElementPtr pNew;
	if ( m_strDefaultNamespace.IsEmpty() )
		pNew = m_pDOMDoc->createElement( _bstr_t(szName) );
	else
		pNew = m_pDOMDoc->createNode( _variant_t((short)MSXMLNS::NODE_ELEMENT),
			_bstr_t(szName), _bstr_t(m_strDefaultNamespace) );

	x_Insert( pParent, pNext, pNew );
	if ( szData && szData[0] )
	{
		MSXMLNS::IXMLDOMNodePtr pText = m_pDOMDoc->createTextNode( _bstr_t(szData) );
		pNew->appendChild( pText );
	}

	if ( bAddChild )
		m_pChild = pNew;
	else
		m_pMain = pNew;
	return TRUE;
}

CString CDOMParser::x_GetSubDoc( MSXMLNS::IXMLDOMNodePtr pNode ) const
{
	if ( (bool)pNode )
		return (LPCTSTR)pNode->xml;
	return _T("");
}

BOOL CDOMParser::x_AddSubDoc( LPCTSTR szSubDoc, BOOL bInsert, BOOL bAddChild )
{
	MSXMLNS::IXMLDOMNodePtr pNext, pParent;
	if ( bAddChild )
	{
		// Add a subdocument under main position, before or after child
		if ( ! (bool)m_pMain )
			return FALSE;
		pParent = m_pMain;
		pNext = m_pChild;
	}
	else
	{
		// Add a subdocument under parent position, before or after main
		if ( ! (bool)m_pParent )
			return FALSE;
		pParent = m_pParent;
		pNext = m_pMain;
	}
	if ( bInsert )
	{
		if ( ! ((bool)pNext) )
			pNext = pParent->GetfirstChild();
	}
	else
	{
		if ( pNext )
			pNext = pNext->GetnextSibling();
	}

#if defined(MARKUP_MSXML4)
	MSXMLNS::IXMLDOMDocument2Ptr pSubDoc;
	pSubDoc.CreateInstance( __uuidof(MSXML2::DOMDocument40) );
#elif defined(MARKUP_MSXML3)
	MSXMLNS::IXMLDOMDocument2Ptr pSubDoc;
	pSubDoc.CreateInstance( __uuidof(MSXML2::DOMDocument) );
#else
	MSXMLNS::IXMLDOMDocumentPtr pSubDoc;
	pSubDoc.CreateInstance( _T("Microsoft.XMLDOM") );
#endif
	// pSubDoc->PutpreserveWhiteSpace( TRUE );
#ifndef _UNICODE
#if defined(MARKUP_MSXML3) || defined(MARKUP_MSXML4)
	VARIANT_BOOL bResult = pSubDoc->loadXML( szSubDoc );
#else
	USES_CONVERSION;
	_bstr_t bstrSubDoc(A2BSTR(szSubDoc),false);
	VARIANT_BOOL bResult = pSubDoc->loadXML( bstrSubDoc );
#endif
#else
	_bstr_t bstrSubDoc(szSubDoc);
	VARIANT_BOOL bResult = pSubDoc->loadXML( bstrSubDoc );
#endif
	if ( ! bResult )
		return FALSE;
	MSXMLNS::IXMLDOMElementPtr pNew = pSubDoc->GetdocumentElement();
	x_Insert( pParent, pNext, pNew );

	if ( bAddChild )
	{
		m_pChild = pNew;
	}
	else
	{
		m_pMain = pNew;
		if ( m_pChild )
			m_pChild.Release();
	}
	return TRUE;
}

BOOL CDOMParser::x_SetAttrib( MSXMLNS::IXMLDOMNodePtr pNode, LPCTSTR szAttrib, int nValue )
{
	_TCHAR szVal[25];
	_itot( nValue, szVal, 10 );
	return x_SetAttrib( pNode, szAttrib, szVal );
}

BOOL CDOMParser::x_SetAttrib( MSXMLNS::IXMLDOMNodePtr pNode, LPCTSTR szAttrib, LPCTSTR szValue )
{
	if ( ! ((bool)pNode) )
		return FALSE;

	MSXMLNS::IXMLDOMNamedNodeMapPtr pAttribs = pNode->Getattributes();
	MSXMLNS::IXMLDOMAttributePtr pAttr = m_pDOMDoc->createAttribute( _bstr_t(szAttrib) );
	if ( pAttr )
	{
		_variant_t varVal;
#ifdef _UNICODE
		varVal.vt = VT_BSTR;
		varVal.bstrVal = SysAllocString(szValue);
#else
		varVal.SetString(szValue);
#endif
		pAttr->put_value( varVal );
		pAttribs->setNamedItem( pAttr );
		return TRUE;
	}
	return FALSE;
}


CString CDOMParser::x_GetData( MSXMLNS::IXMLDOMNodePtr pNode ) const
{
	CString strData;
	if ( ! ((bool)pNode) )
		return strData;


	if ( (bool)(pNode->GetfirstChild()) )
		strData = (LPCTSTR) pNode->Gettext();
	return strData;
}

BOOL CDOMParser::x_SetData( MSXMLNS::IXMLDOMNodePtr& pNode, LPCTSTR szData, int nCDATA )
{
	if ( ! ((bool)pNode) )
		return FALSE;


	if ( ! ((bool)pNode) )
		return FALSE;

	// Return FALSE if child element
	MSXMLNS::IXMLDOMNodePtr pChild = pNode->GetfirstChild();
	MSXMLNS::IXMLDOMNodePtr pChildNext = pChild;
	while ( (bool)pChildNext )
	{
		if ( pChildNext->nodeType == MSXMLNS::NODE_ELEMENT )
			return FALSE;
		pChildNext = pChildNext->GetnextSibling();
	}

	// Remove any child nodes
	pChildNext = pChild;
	while ( (bool)pChildNext )
	{
		MSXMLNS::IXMLDOMNodePtr pChildDel = pChildNext;
		pChildNext = pChildNext->GetnextSibling();
		pNode->removeChild( pChildDel );
	}

	if ( nCDATA != 0 )
	{
		MSXMLNS::IXMLDOMCDATASectionPtr pData;
		LPCTSTR pszNextStart = szData;
		LPCTSTR pszEnd = _tcsstr( szData, _T("]]>") );
		while ( pszEnd )
		{
			CString csSegment( pszNextStart, (int)(pszEnd - pszNextStart) + 2 );
			pData = m_pDOMDoc->createCDATASection( _bstr_t((LPCTSTR)csSegment) );
			pNode->appendChild( pData );
			pszNextStart = pszEnd + 2;
			pszEnd = _tcsstr( pszNextStart, _T("]]>") );
		}
		pData = m_pDOMDoc->createCDATASection( _bstr_t(pszNextStart) );
		pNode->appendChild( pData );
	}
	else
	{
		MSXMLNS::IXMLDOMNodePtr pText = m_pDOMDoc->createTextNode( _bstr_t(szData) );
		pNode->appendChild( pText );
	}

	return TRUE;
}
#endif