
// BPDlg.h : ͷ�ļ�
//

#pragma once

#include "BPANN.h"

// CBPDlg �Ի���
class CBPDlg : public CDialogEx
{
// ����
public:
	CBPDlg(CWnd* pParent = NULL);	// ��׼���캯��
	CString train_filepath;
	clsANN ANN;
// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BP_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonOpen();
	afx_msg void OnBnClickedButtonTrain();
	afx_msg void OnBnClickedButtonCompute();
	afx_msg void OnBnClickedButtonSaveNet();
	afx_msg void OnBnClickedButtonLoadNet();
	afx_msg void OnBnClickedButtonCalculate();
	double input_x1;
	double input_x2;
	double output_y;
};
