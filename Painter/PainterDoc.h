// This MFC Samples source code demonstrates using MFC Microsoft Office Fluent User Interface 
// (the "Fluent UI") and is provided only as referential material to supplement the 
// Microsoft Foundation Classes Reference and related electronic documentation 
// included with the MFC C++ library software.  
// License terms to copy, use or distribute the Fluent UI are available separately.  
// To learn more about our Fluent UI licensing program, please visit 
// http://go.microsoft.com/fwlink/?LinkId=238214.
//
// Copyright (C) Microsoft Corporation
// All rights reserved.
// PainterDoc.h : interface of the CPainterDoc class

#pragma once

#include <vector>
#include <memory>

class CShape;
class CLine;
class CRectangle;
class CShapeFactory;

class CPainterDoc : public CDocument
{
protected: // create from serialization only
	CPainterDoc();
	DECLARE_DYNCREATE(CPainterDoc)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	bool Group();
	bool Ungroup();
	bool Cut();

	void Copy();
	void Paste();
	bool CanCopy();
	bool CanPaste();

#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	virtual ~CPainterDoc();
	const std::vector<std::shared_ptr<CShape>>& GetShapes() const;
	std::vector<std::shared_ptr<CShape>>& Shapes();

	void AddShape(std::shared_ptr<CShape> shape);

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS

private:
	std::vector<std::shared_ptr<CShape>> _shapes;
	std::shared_ptr<CShapeFactory> _shape_factory;

};
