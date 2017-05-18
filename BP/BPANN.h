#pragma once

#include <math.h>
#include <time.h>
#include <conio.h>

using namespace std;

#define INPUT_DIM 2 //�����������
#define OUTPUT_DIM 1 //�����������
#define HIDE_DIM 45 //�����ڵ����

#define SAMPLE_NUM 1000 //ѵ����������


struct structNode
{
	double weight[50]; //��ǰ�ڵ��ÿһ�������Ȩ��
	double dweight[50]; //ÿ��ѵ����ÿ��Ȩ����Ҫ�ı����
	double delta; //����ÿ��ڵ�Ȩ�ظı���ʱ�õ���ϵ��
	double sum_dw; //����������ڵ�Ȩ�ظı���ʱ�õ���ϵ��
	double Output_x;//��ǰ�ڵ�����֮��
	double Output_y; //��ǰ�ڵ����
	double Output_dy; //��ǰ�ڵ�����ĵ���
};
typedef struct structNode strctNode; //�ڵ�ṹ��

struct structData
{
	double TrainData[SAMPLE_NUM]; //���ݣ�ԭʼ�ġ���һ�����
	double TrainData_Max; //�������ֵ����һ����
	double TrainData_Min; //������Сֵ����һ����
	double TrainData_MaxMin; //�������ֵ����Сֵ֮���һ����
};
typedef struct structData strctData; //���ݣ������������ṹ��

class clsANN
{
public:
	double error_limit; //����ޣ����С������ֹͣѵ��
	double error; //��ǰ���
	double error_max; //��ǰ������
	double rate; //ѧϰ��

	bool is_trained; //������֤��ʱ���õģ��ǲ����Ѿ���һ������
	
	strctNode NodeOutput[OUTPUT_DIM]; //����ڵ�
	strctNode NodeHide[HIDE_DIM]; //������ڵ�
	                                                    //����ڵ�ֱ�Ӿ�������
	strctData TrainDataIn[INPUT_DIM]; //��������ѵ������
	strctData TrainDataOut[OUTPUT_DIM]; //�������ѵ������
	double ResultOut[OUTPUT_DIM][SAMPLE_NUM]; //���������������Ľ��

	void do_train(double rate, double error_limit, int max_iteration); //ѵ������
	void calc_result(); //������֤����
	double calc_test(double x[]); //���Ժ���

	void init_weight(); //ʹ���������ʼ��Ȩ��
	                             
	void findmaxmin(); //�������Сֵ
	void input_normalization(int num_sample); //�������ݹ�һ��
	void output_normalization(int num_sample); //������ݹ�һ��
	void output_anti_normalization(int num_sample); //�������������һ��

	void calc_output(int group_sample); //���㵱ǰ���
	void calc_dweight(int group_sample); //���㵱ǰȨ����Ҫ�ı����
	void adjust_weight(); //����Ȩ��
};

