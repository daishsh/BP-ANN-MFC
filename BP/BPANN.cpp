#include "stdafx.h"
#include "BPANN.h"

void clsANN::init_weight() //��ʼ��Ȩ��Ϊ-1~1֮������������ʼ��Ȩ�ر仯������0
{
	int i, j;
	for (i = 0; i < OUTPUT_DIM; i++)
	{
		for (j = 0; j < HIDE_DIM; j++)
		{
			NodeOutput[i].dweight[j] = 0;
			NodeOutput[i].weight[j] = (rand() * 2.0 / RAND_MAX) - 1; //rand()����0~RAND_MAX֮��������
		}
	}
	for (i = 0; i < HIDE_DIM; i++)
	{
		for (j = 0; j < INPUT_DIM; j++)
		{
			NodeHide[i].dweight[j] = 0;
			NodeHide[i].weight[j] = (rand() * 2.0 / RAND_MAX) - 1;//rand()����0~RAND_MAX֮��������
			NodeHide[i].sum_dw = 0;
		}
	}
}

void clsANN::findmaxmin() //��������������ֵ����Сֵ���������ֵ����Сֵ֮��
{
	int i, j;
	for (i = 0; i < INPUT_DIM; i++)
	{
		TrainDataIn[i].TrainData_Max = TrainDataIn[i].TrainData[0];
		TrainDataIn[i].TrainData_Min = TrainDataIn[i].TrainData[0];
		for (j = 0; j < SAMPLE_NUM; j++)
		{
			if (TrainDataIn[i].TrainData[j] > TrainDataIn[i].TrainData_Max) TrainDataIn[i].TrainData_Max = TrainDataIn[i].TrainData[j];
			if (TrainDataIn[i].TrainData[j] < TrainDataIn[i].TrainData_Min) TrainDataIn[i].TrainData_Min = TrainDataIn[i].TrainData[j];
		}
		TrainDataIn[i].TrainData_MaxMin = TrainDataIn[i].TrainData_Max - TrainDataIn[i].TrainData_Min + 1;
	}

	for (i = 0; i < OUTPUT_DIM; i++)
	{
		TrainDataOut[i].TrainData_Max = TrainDataOut[i].TrainData[0];
		TrainDataOut[i].TrainData_Min = TrainDataOut[i].TrainData[0];
		for (j = 0; j < SAMPLE_NUM; j++)
		{
			if (TrainDataOut[i].TrainData[j] > TrainDataOut[i].TrainData_Max) TrainDataOut[i].TrainData_Max = TrainDataOut[i].TrainData[j];
			if (TrainDataOut[i].TrainData[j] < TrainDataOut[i].TrainData_Min) TrainDataOut[i].TrainData_Min = TrainDataOut[i].TrainData[j];
		}
		TrainDataOut[i].TrainData_MaxMin = TrainDataOut[i].TrainData_Max - TrainDataOut[i].TrainData_Min + 1;
	}
}

void clsANN::input_normalization(int num_sample) //��һ������
{
	int i, j;
	for (i = 0; i < INPUT_DIM; i++)
	{
		for (j = 0; j < num_sample; j++)
		{
			TrainDataIn[i].TrainData[j] = (TrainDataIn[i].TrainData[j] - TrainDataIn[i].TrainData_Min + 1) / TrainDataIn[i].TrainData_MaxMin;
		}
	}
}

void clsANN::output_normalization(int num_sample) //��һ�����
{
	int i, j;
	for (i = 0; i < OUTPUT_DIM; i++)
	{
		for (j = 0; j < num_sample; j++)
		{
			TrainDataOut[i].TrainData[j] = (TrainDataOut[i].TrainData[j] - TrainDataOut[i].TrainData_Min + 1) / TrainDataOut[i].TrainData_MaxMin;
		}
	}
}

void clsANN::output_anti_normalization(int num_sample) //�������ȥ��һ��
{
	int i;
	for (i = 0; i < OUTPUT_DIM; i++)
	{
		ResultOut[i][num_sample] = NodeOutput[i].Output_y  * TrainDataOut[i].TrainData_MaxMin + TrainDataOut[i].TrainData_Min - 1;
	}
}

void clsANN::calc_output(int group_sample) //�����������
{
	int i, j;
	for (i = 0; i < HIDE_DIM; i++) //����������ڵ�����������㼤���ʹ��sigmoid����
	{
		NodeHide[i].Output_x = 0;
		for (j = 0; j < INPUT_DIM; j++)
		{
			NodeHide[i].Output_x += NodeHide[i].weight[j] * TrainDataIn[j].TrainData[group_sample];
		}
		NodeHide[i].Output_y = 1.0 / (1.0 + exp((-1.0) * NodeHide[i].Output_x));
		NodeHide[i].Output_dy = 1.0 * NodeHide[i].Output_y * (1.0 - NodeHide[i].Output_y);
	}
	for (i = 0; i < OUTPUT_DIM; i++) //���������ڵ����������㼤���ֱ��ʹ��y = x
	{
		NodeOutput[i].Output_x = 0;
		for (j = 0; j < HIDE_DIM; j++)
		{
			NodeOutput[i].Output_x += NodeOutput[i].weight[j] * NodeHide[j].Output_y;
		}
		NodeOutput[i].Output_y = NodeOutput[i].Output_x; //1.0 / (1.0 + exp((-1.0) * NodeOutput[i].Output_x)); //
		NodeOutput[i].Output_dy = 1.0 * NodeOutput[i].Output_y * (1.0 - NodeOutput[i].Output_y);
		error += fabs((TrainDataOut[i].TrainData[group_sample] - NodeOutput[i].Output_y) / TrainDataOut[i].TrainData[group_sample]);
		//if (error > error_max) error_max = error;
	}
}

void clsANN::calc_dweight(int group_sample) //���򴫲�������Ȩ�ص���ֵ
{
	int i, j;
	double delta;
	for (i = 0; i < OUTPUT_DIM; i++) //���չ�ʽ���������ڵ��Ȩ�ص���ֵ���ۼ�ÿ������ڵ������delta*weight֮�����ڼ�������ڵ��Ȩ�ص���ֵ
	{
		delta = (TrainDataOut[i].TrainData[group_sample] - NodeOutput[i].Output_y);//* NodeOutput[i].Output_dy;
		for (j = 0; j <HIDE_DIM; j++)
		{
			NodeHide[j].sum_dw += delta * NodeOutput[i].weight[j];
			NodeOutput[i].dweight[j] += rate * delta * NodeHide[j].Output_y;
		}
	}

	for (i = 0; i < HIDE_DIM; i++) //���չ�ʽ��������ڵ��Ȩ�ص���ֵ
	{
		delta = NodeHide[i].sum_dw * NodeHide[i].Output_dy;
		NodeHide[i].sum_dw = 0;
		for (j = 0; j <INPUT_DIM; j++)
		{
			NodeHide[i].dweight[j] += rate * delta * TrainDataIn[j].TrainData[group_sample];
		}
	}
}

void clsANN::adjust_weight() //����Ȩ��
{
	int i, j;
	for (i = 0; i < OUTPUT_DIM; i++)
	{
		for (j = 0; j < HIDE_DIM; j++)
		{
			NodeOutput[i].weight[j] += NodeOutput[i].dweight[j];
			NodeOutput[i].dweight[j] = 0;
		}
	}

	for (i = 0; i < HIDE_DIM; i++)
	{
		for (j = 0; j < INPUT_DIM; j++)
		{
			NodeHide[i].weight[j] += NodeHide[i].dweight[j];
			NodeHide[i].dweight[j] = 0;
		}
	}
}

void clsANN::do_train(double r, double e, int max_iteration) //ѵ������
{
	_cprintf("train\r\n");
	double error_pre = 0;
	int i, j;
	srand((unsigned)time(NULL));
	rate = r;
	error_limit = e;
	init_weight(); //��ʼ��Ȩ��
	findmaxmin(); //�������Сֵ
	input_normalization(SAMPLE_NUM); //��һ������
	output_normalization(SAMPLE_NUM); //��һ�����
	for (i = 0; 0 < max_iteration; i++) //������˵Ӧ������������������������Ϊ���һֱ�ﲻ��Ҫ�������������������������
	{
		error = 0; //������˵Ӧ���ж�������ֵ������������ֵѵ����һ����Ҳû�����ٷ�֮��һ�����ֻ����ƽ��ֵ
		for (j = 0; j < SAMPLE_NUM; j++) //ÿһ�����������������ʹ��BP�㷨�����������ֵ�����ۼ�
		{
			calc_output(j);
			calc_dweight(j);
		}
		error = error / SAMPLE_NUM; //���ƽ��ֵ
		if (error <= error_limit) break; //ƽ��ֵС�ڰٷ�֮��ѵ��ֹͣ
		if (error > 1) //�Լ��Բ���һ������ѧϰ�ʵĸĽ�����
		{
			if (error > error_pre || error_pre - error < 0.005) //���ѵ�������������˻�������½���̫�������ѧϰ��
			rate = r * (rand() * 1.0 / RAND_MAX + 0.5); //�������һ����Χ��1~1.5�����ӳ����趨��ѧϰ�ʣ��������������߼�Сѧϰ��
		}
		else rate = r; //�������Ѿ�����1���¾Ͳ������ڣ�����ѵ��
		error_pre = error;
		_cprintf("%d %lf\r\n", i, error);
		adjust_weight(); //����һ��Ȩ��
	}
	is_trained = true; //ѵ�����ˣ���־��1
}

void clsANN::calc_result() //��ѵ���������ص������м�����������֤
{
	_cprintf("simulate\r\n");
	if (is_trained == false)
	{
		findmaxmin();
		input_normalization(SAMPLE_NUM);
	}
	for (int j = 0; j < SAMPLE_NUM; j++)
	{
		calc_output(j);
		output_anti_normalization(j);
		if (is_trained == false) _cprintf("real:%lf result:%lf\r\n", TrainDataOut[0].TrainData[j], ResultOut[0][j]);
		else _cprintf("real:%lf result:%lf\r\n", TrainDataOut[0].TrainData[j] * TrainDataOut[0].TrainData_MaxMin + TrainDataOut[0].TrainData_Min - 1, ResultOut[0][j]);
	}
}

double clsANN::calc_test(double x[]) //������������������֤
{
	double y;
	for (int i = 0; i < INPUT_DIM; i++)
	{
		x[i] = (x[i] - TrainDataIn[i].TrainData_Min + 1) / (TrainDataIn[i].TrainData_Max - TrainDataIn[i].TrainData_Min + 1);
	}
	for (int i = 0; i < HIDE_DIM; i++)
	{
		NodeHide[i].Output_x = 0;
		for (int j = 0; j < INPUT_DIM; j++)
		{
			NodeHide[i].Output_x += NodeHide[i].weight[j] * x[j];
		}
		NodeHide[i].Output_y = 1.0 / (1.0 + exp((-1.0) * NodeHide[i].Output_x));
		NodeHide[i].Output_dy = 1.0 * NodeHide[i].Output_y * (1.0 - NodeHide[i].Output_y);
	}
	for (int i = 0; i < OUTPUT_DIM; i++)
	{
		NodeOutput[i].Output_x = 0;
		for (int j = 0; j < HIDE_DIM; j++)
		{
			NodeOutput[i].Output_x += NodeOutput[i].weight[j] * NodeHide[j].Output_y;
		}
		NodeOutput[i].Output_y = NodeOutput[i].Output_x; //1.0 / (1.0 + exp((-1.0) * NodeOutput[i].Output_x)); 
	}
	y = NodeOutput[0].Output_y  * (TrainDataOut[0].TrainData_Max - TrainDataOut[0].TrainData_Min + 1) + TrainDataOut[0].TrainData_Min - 1;
	return y;
}