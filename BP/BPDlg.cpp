
// BPDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "BP.h"
#include "BPDlg.h"
#include "afxdialogex.h"
#include "BPANN.h"

#include <iostream>
#include <fstream>
using namespace std;
#include <conio.h>
#include <direct.h>
#include <ctype.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CBPDlg �Ի���
//clsANN ANN;


CBPDlg::CBPDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_BP_DIALOG, pParent)
	, input_x1(0)
	, input_x2(0)
	, output_y(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CBPDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_X1, input_x1);
	DDX_Text(pDX, IDC_EDIT_X2, input_x2);
	DDX_Text(pDX, IDC_EDIT_Y, output_y);
}

BEGIN_MESSAGE_MAP(CBPDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_OPEN, &CBPDlg::OnBnClickedButtonOpen)
	ON_BN_CLICKED(IDC_BUTTON_TRAIN, &CBPDlg::OnBnClickedButtonTrain)
	ON_BN_CLICKED(IDC_BUTTON_COMPUTE, &CBPDlg::OnBnClickedButtonCompute)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_NET, &CBPDlg::OnBnClickedButtonSaveNet)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_NET, &CBPDlg::OnBnClickedButtonLoadNet)
	ON_BN_CLICKED(IDC_BUTTON_CALCULATE, &CBPDlg::OnBnClickedButtonCalculate)
END_MESSAGE_MAP()


// CBPDlg ��Ϣ�������

BOOL CBPDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	AllocConsole();
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CBPDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CBPDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CBPDlg::OnBnClickedButtonOpen()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString filename;
	char *path;
	CString str;
	FILE *fp1, *fp2;
	CFileDialog	FileDlg(true);

	FileDlg.m_ofn.lpstrTitle = _T("����������ѵ����");
	FileDlg.m_ofn.lpstrFilter =
		_T("�ı��ĵ�(*.txt)\0*.txt\0�����ļ�(*.*)\0*.*\0\0");
	if (FileDlg.DoModal() == IDOK)
	{ 
		filename = FileDlg.GetPathName();
		USES_CONVERSION;
		path = T2A(filename);
		
		CWnd::SetDlgItemTextW(IDC_STATIC_INPUT_PATH, filename);
		
		if ((fp1 = fopen(path, "r")) == NULL)
		{
				printf("can not open the in file\n");
				exit(0);
		}
			
		for (int i = 0; i < SAMPLE_NUM; i++)
		{
			for (int j = 0; j < INPUT_DIM; j++)
			{
				fscanf(fp1, "%lf", &ANN.TrainDataIn[j].TrainData[i]);
			}
		}
		fclose(fp1);
	}

	FileDlg.m_ofn.lpstrTitle = _T("���������ѵ����");
	FileDlg.m_ofn.lpstrFilter =
		_T("�ı��ĵ�(*.txt)\0*.txt\0�����ļ�(*.*)\0*.*\0\0");
	if (FileDlg.DoModal() == IDOK)
	{
		filename = FileDlg.GetPathName();
		USES_CONVERSION;
		path = T2A(filename);

		CWnd::SetDlgItemTextW(IDC_STATIC_OUTPUT_PATH, filename);

		if ((fp2 = fopen(path, "r")) == NULL)
		{
			printf("can not open the in file\n");
			exit(0);
		}

		for (int i = 0; i < SAMPLE_NUM; i++)
		{
			for (int j = 0; j < OUTPUT_DIM; j++)
			{
				fscanf(fp2, "%lf", &ANN.TrainDataOut[j].TrainData[i]);
			}
		}
		fclose(fp2);
	}
	ANN.is_trained = false;
}


void CBPDlg::OnBnClickedButtonTrain()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	ANN.do_train(0.0003, 0.01, 40000); //ѧϰ�ʣ�����޶ȣ�����������
}

void CBPDlg::OnBnClickedButtonCompute()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	ANN.calc_result();
}


void CBPDlg::OnBnClickedButtonSaveNet()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString filename;
	char *path;
	CString str;
	FILE *fp1, *fp2;
	CFileDialog	FileDlg(true);

	FileDlg.m_ofn.lpstrTitle = _T("���浱ǰȨ��");
	FileDlg.m_ofn.lpstrFilter =
		_T("�ı��ĵ�(*.txt)\0*.txt\0�����ļ�(*.*)\0*.*\0\0");
	if (FileDlg.DoModal() == IDOK)
	{
		filename = FileDlg.GetPathName();
		USES_CONVERSION;
		path = T2A(filename);

		CWnd::SetDlgItemTextW(IDC_STATIC_NET_PATH, filename);

		if ((fp1 = fopen(path, "w")) == NULL)
		{
			printf("can not save the file\n");
			exit(0);
		}

		for (int i = 0; i < HIDE_DIM; i++)
		{
			for (int j = 0; j < INPUT_DIM; j++)
			{
				fprintf(fp1, "%lf ", ANN.NodeHide[i].weight[j]);
			}
		}

		for (int i = 0; i < OUTPUT_DIM; i++)
		{
			for (int j = 0; j < HIDE_DIM; j++)
			{
				fprintf(fp1, "%lf ", ANN.NodeOutput[i].weight[j]);
			}
		}

		//��һʱ�õĲ���Ҳ��һ��
		for (int i = 0; i < INPUT_DIM; i++)
		{
			fprintf(fp1, "%lf ", ANN.TrainDataIn[i].TrainData_Max);
			fprintf(fp1, "%lf ", ANN.TrainDataIn[i].TrainData_Min);
		}

		for (int i = 0; i < OUTPUT_DIM; i++)
		{
			fprintf(fp1, "%lf ", ANN.TrainDataOut[i].TrainData_Max);
			fprintf(fp1, "%lf ", ANN.TrainDataOut[i].TrainData_Min);
		}

		fclose(fp1);
	}
}


void CBPDlg::OnBnClickedButtonLoadNet()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString filename;
	char *path;
	CString str;
	FILE *fp1, *fp2;
	CFileDialog	FileDlg(true);

	FileDlg.m_ofn.lpstrTitle = _T("������");
	FileDlg.m_ofn.lpstrFilter =
		_T("�ı��ĵ�(*.txt)\0*.txt\0�����ļ�(*.*)\0*.*\0\0");
	if (FileDlg.DoModal() == IDOK)
	{
		filename = FileDlg.GetPathName();
		USES_CONVERSION;
		path = T2A(filename);

		CWnd::SetDlgItemTextW(IDC_STATIC_NET_PATH, filename);

		if ((fp1 = fopen(path, "r")) == NULL)
		{
			printf("can not open the in file\n");
			exit(0);
		}

		for (int i = 0; i < HIDE_DIM; i++)
		{
			for (int j = 0; j < INPUT_DIM; j++)
			{
				fscanf(fp1, "%lf ", &ANN.NodeHide[i].weight[j]);
			}
		}

		for (int i = 0; i < OUTPUT_DIM; i++)
		{
			for (int j = 0; j < HIDE_DIM; j++)
			{
				fscanf(fp1, "%lf ", &ANN.NodeOutput[i].weight[j]);
			}
		}

		for (int i = 0; i < INPUT_DIM; i++)
		{
			fscanf(fp1, "%lf ", &ANN.TrainDataIn[i].TrainData_Max);
			fscanf(fp1, "%lf ", &ANN.TrainDataIn[i].TrainData_Min);
		}

		for (int i = 0; i < OUTPUT_DIM; i++)
		{
			fscanf(fp1, "%lf ", &ANN.TrainDataOut[i].TrainData_Max);
			fscanf(fp1, "%lf ", &ANN.TrainDataOut[i].TrainData_Min);
		}

		fclose(fp1);
	}
}


void CBPDlg::OnBnClickedButtonCalculate()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	double x[2];
	UpdateData(TRUE);
	x[0] = input_x1;
	x[1] = input_x2;
	output_y = ANN.calc_test(x);
	UpdateData(FALSE);
}
