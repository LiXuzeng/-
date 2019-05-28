#pragma warning(disable:4996)
#include<stdio.h>
#include<stdlib.h>

struct Page
{
	int pagenum;//ҳ��
	int blocknum;//���洢�Ŀ��
	int presence = 0;//����λ����ʼΪ0��ʾ������
	int modify = 0;//�޸�λ����ʼλ0����ʾδ���޸�
	int visit = 0;//����λ����ʼֵδ0��ʾδ������
};
//Page page[64];//ģ��ҳ������
//int BLOCK[32] = {-1};//ά���������,��ʼֵΪ-1��ʾ����Ϊ�գ�ֵΪ���ڴ��ڵ�ҳ��
int free_block[6];//���п飬����PBA�㷨
int get_pageplace(int a, Page page[40000], int end)
{
	for (int i = 0; i < end; i++)
	{
		if (page[i].pagenum == a)
			return i;
	}
}
int find_all(int a, int BLOCK[32])
{
	for (int i = 0; i < 32; i++)
	{
		if (a == BLOCK[i])
			return 1;
	}
	return 0;
}
int find_optimal(int a, int begin, Page page[1000])
{
	int i = begin;
	int flag = 1;
	for (i; i < 30000; i++)
	{
		if (page[i].pagenum == a)
			return i;
		else
			flag = 0;
	}
	if (flag == 0)
	{
		return -1;
	}
}
void optimal(int size, Page page[30000], int BLOCK[32])
{
	int num = 0;
	int deletion = 0;
	while (num < size)
	{
		if (find_all(page[num].pagenum, BLOCK))
		{
			num++;
		}
		else
		{
			int tag = 0;
			for (int i = 0; i < 32; i++)
			{
				if (BLOCK[i] == -1)//�ҵ����п�ֱ�Ӵ洢
				{
					BLOCK[i] = page[num].pagenum;
					page[num].blocknum = i;
					num++;
					tag = 1;
					i = 32;
				}
			}
			if (tag == 0)//δ�ҵ����п�
			{
				int b = 0;
				int change = -1;//�����滻���ݵ�ҳλ��
				int page_c;//�����滻��ҳ��
				while (b < 32)
				{
					int temp = find_optimal(BLOCK[b], num, page);
					if (temp > change)//���ϵ����ҵ����δʹ�õ�ҳ���
					{
						change = temp;
						page_c = page[change].pagenum;
					}
					if (temp == -1)
					{
						page_c = BLOCK[b];
						b = 64;//����ѭ�������ҵ��Ժ󲻻�ʹ�õ�ҳ��
					}
					b++;
				}
				for (int i = 0; i < 32; i++)//�ҵ���Ӧ�����ڴ���Ҫ�滻��ҳ
				{
					if (BLOCK[i] == page_c)
					{
						BLOCK[i] = page[num].pagenum;
						page[num].blocknum = i;
					}
				}
				deletion++;
				num++;
			}
		}
	}
	double rate = (double(deletion) / double(size));
	printf("����û��㷨��*****ȱҳ������%d��ȱҳ�ʣ�%.3f\n", deletion, rate);
}

void fifo(int size, Page page[30000], int BLOCK[32])
{
	int num = 0;
	int deletion = 0;
	while (num < size)
	{
		if (find_all(page[num].pagenum, BLOCK))
		{
			num++;
		}
		else
		{
			int tag = 0;
			for (int i = 0; i < 32; i++)
			{
				if (BLOCK[i] == -1)//�ҵ����п�ֱ�Ӵ洢
				{
					BLOCK[i] = page[num].pagenum;
					page[num].blocknum = i;
					num++;
					tag = 1;
					i = 32;
				}
			}
			if (tag == 0)//δ�ҵ����п�
			{
				int b = 0;
				int change = 40000;//�����滻���ݵ�ҳλ��
				while (b < 32)
				{
					for (int j = 0; j < num; j++)
					{
						if (page[j].pagenum == BLOCK[b])
						{
							if (j < change)
								change = j;
						}
					}
					b++;
				}
				for (int i = 0; i < 32; i++)//�ҵ���Ӧ�����ڴ���Ҫ�滻��ҳ
				{
					if (BLOCK[i] == page[change].pagenum)
					{
						BLOCK[i] = page[num].pagenum;
						page[num].blocknum = i;
					}
				}
				deletion++;
				num++;
			}
		}
	}
	double rate = (double(deletion) / double(size));
	printf("�Ƚ��ȳ��û��㷨��******ȱҳ������%d��ȱҳ�ʣ�%.3f\n", deletion, rate);
}

void LRU(int size, Page page[30000], int BLOCK[32])
{
	int num = 0;
	int deletion = 0;
	while (num < size)
	{
		if (find_all(page[num].pagenum, BLOCK))
		{
			num++;
		}
		else
		{
			int tag = 0;
			for (int i = 0; i < 32; i++)
			{
				if (BLOCK[i] == -1)//�ҵ����п�ֱ�Ӵ洢
				{
					BLOCK[i] = page[num].pagenum;
					page[num].blocknum = i;
					num++;
					tag = 1;
					i = 32;
				}
			}
			if (tag == 0)//δ�ҵ����п�
			{
				int b = 0;
				int change = 40000;//�����滻���ݵ�ҳλ��
				while (b < 32)
				{
					for (int j = num; j >= 0; j--)//�ӵ�ǰλ����ǰ�����һ��ʹ�õļ�¼
					{
						if (page[j].pagenum == BLOCK[b])
						{
							if (j < change)
								change = j;
							j = -1;//��һ���ҵ�����ѭ��
						}
					}
					b++;
				}
				for (int i = 0; i < 32; i++)//�ҵ���Ӧ�����ڴ���Ҫ�滻��ҳ
				{
					if (BLOCK[i] == page[change].pagenum)
					{
						BLOCK[i] = page[num].pagenum;
						page[num].blocknum = i;
					}
				}
				deletion++;
				num++;
			}
		}
	}
	double rate = (double(deletion) / double(size));
	printf("LRU�û��㷨��*****ȱҳ������%d��ȱҳ�ʣ�%.3f\n", deletion, rate);
}

void clock(int size, Page page[30000], int BLOCK[32])
{
	int num = 0;
	int deletion = 0;
	while (num < size)
	{
		if (find_all(page[num].pagenum, BLOCK))//�����е������ڴ����ҵ�ҳ���޸������λΪ1
		{
			page[num].visit = 1;
			num++;
		}
		else
		{
			int tag = 0;
			for (int i = 0; i < 32; i++)
			{
				if (BLOCK[i] == -1)//�ҵ����п�ֱ�Ӵ洢
				{
					BLOCK[i] = page[num].pagenum;
					page[num].blocknum = i;
					num++;
					tag = 1;
					i = 32;
				}
			}
			if (tag == 0)//δ�ҵ����п�
			{
				int b = 0;
				int flag = 0;//�ж��Ƿ��ҵ��滻ҳ�棬��ʼֵΪ0��ʾδ�ҵ�
				int change = 40000;//�����滻ҳ������λ��
				while (!flag)//ֱ���ҵ�Ϊֹ
				{
					for (int i = 0; i < 32 && !flag; i++)
					{
						int place = get_pageplace(BLOCK[i], page, num);
						if (page[place].visit == 0 && page[place].modify == 0)
						{
							if (i < change)//�ʼ�ҵ���Ϊ�滻λ��
							{
								change = i;
								flag = 1;
							}
						}
					}
					if (flag != 1)
					{
						for (int i = 0; i < 32 && !flag; i++)
						{
							int place2 = get_pageplace(BLOCK[i], page, num);
							if (page[place2].visit == 0 && page[place2].modify == 1)
							{
								page[place2].visit = 0;//�����з��ʵ���ҳ�����λ����0
								if (i < change)//�ʼ�ҵĵ�Ϊ�滻λ��
								{
									change = i;
									flag = 1;
								}
							}
						}
					}
				}
				BLOCK[change] = page[num].pagenum;//ֱ�����滻λ���滻
				deletion++;
				num++;
			}
		}
	}
	double rate = (double(deletion) / double(size));
	printf("�Ľ�clock�㷨��****ȱҳ������%d��ȱҳ�ʣ�%.3f\n", deletion, rate);
}

void PBA(int size, Page page[30000], int BLOCK[32], int freeblock[2])//freeblockΪ���п飬�����滻�������Ŀ�
{
	freeblock[0] = -1;
	freeblock[1] = -1;//��ʼ��Ϊ-1��ʾû�д洢����
	int num = 0;
	int deletion = 0;
	while (num < size)
	{
		if (find_all(page[num].pagenum, BLOCK))
		{
			num++;
		}
		else
		{
			int tag = 0;
			for (int i = 0; i < 32; i++)
			{
				if (BLOCK[i] == -1)//�ҵ����п�ֱ�Ӵ洢
				{
					BLOCK[i] = page[num].pagenum;
					page[num].blocknum = i;
					tag = 1;
					i = 32;
					num++;
				}
			}
			if (tag == 0)//δ�ҵ����п�
			{
				int b = 0;
				int change = 40000;//�����滻���ݵ�ҳλ��
				while (b < 32)
				{
					for (int j = 0; j < num; j++)
					{
						if (page[j].pagenum == BLOCK[b])
						{
							if (j < change)
								change = j;
						}
					}
					b++;
				}
				deletion++;
				num++;
				for (int i = 0; i < 32; i++)//�ҵ���Ӧ�����ڴ���Ҫ�滻��ҳ
				{
					int exit = 0;
					if (BLOCK[i] == page[change].pagenum)
					{
						for (int j = 0; j < 2; j++)//�Ӵ洢�û�����ҳ��Ŀ��п���Ѱ���Ƿ����Ҫ�û���ҳ��
						{
							if (freeblock[j] == page[change].pagenum)//������ڣ�ֱ���û��飬���������Ŀ������п�Ķ���
							{
								freeblock[j] = page[change].pagenum;
								BLOCK[i] = freeblock[j];
								exit = 1;
							}
						}
						if (exit == 0);
						{
							for (int j = 0; j < 2; j++)
							{
								if (freeblock[j] == -1)
								{
									exit = 1;
									freeblock[j] = BLOCK[i];
									BLOCK[i] = page[num].pagenum;
									page[num].blocknum = i;
								}
							}
						}
						if (exit == 0)
						{
							freeblock[0] = BLOCK[i];
							BLOCK[i] = page[num].pagenum;
							page[num].blocknum = i;
						}
					}
				}
			}
		}
	}
	double rate = (double(deletion) / double(size));
	printf("PBA�㷨��****ȱҳ������%d��ȱҳ�ʣ�%.3f\n", deletion, rate);
}

int product(int b[30000], int N, int p, int e, int m, double t)
{
	int flag = 19;
	int place = 0;
	while (flag)
	{	
		for (int i = 0; i < m; i++,place++)
		{
			b[place] = (abs(rand()) % e) + p;
		}
		double r = (abs(rand()) % 100) / 100;
		if (r < t)
		{
			p = abs(rand() % N);
		}
		else
		{
			p = (p + 1) % N;
		}
		flag--;
		//printf("��������Ӵ�ҳ��������д��ĳ��������룺1,�������룺0\n");
		//scanf("%d", &flag);
	}
	return place;
}
int main()
{
	int temp[30000];
	int freeblock[2];
	int size;
	Page p_t[30000];//ģ��ҳ������
	int block[32];//ά���������,��ʼֵΪ-1��ʾ����Ϊ�գ�ֵΪ���ڴ��ڵ�ҳ�桢
	for (int i = 0; i < 32; i++)
	{
		block[i] = -1;
	}
	char flag = 'b';
	size = product(temp, 200, 0, 15, 1000, 0.5);
	while (flag != 'e')
	{
		printf("�������룺b,�˳����룺e\n");
		scanf("%c",&flag);
		{
			for (int i = 0; i < 30000; i++)
			{
				p_t[i].pagenum = temp[i];
				p_t[i].visit = 0;
				p_t[i].modify = 0;
				p_t[i].blocknum = -1;
				p_t[i].presence = 0;
			}
			for (int i = 0; i < 32; i++)
			{
				block[i] = -1;
			}
			optimal(size, p_t, block);
		}
		{
			for (int i = 0; i < 30000; i++)
			{
				p_t[i].pagenum = temp[i];
				p_t[i].visit = 0;
				p_t[i].modify = 0;
				p_t[i].blocknum = -1;
				p_t[i].presence = 0;
			}
			for (int i = 0; i < 32; i++)
			{
				block[i] = -1;
			}
			fifo(size, p_t, block);
		}
		{
			for (int i = 0; i < 30000; i++)
			{
				p_t[i].pagenum = temp[i];
				p_t[i].visit = 0;
				p_t[i].modify = 0;
				p_t[i].blocknum = -1;
				p_t[i].presence = 0;
			}
			for (int i = 0; i < 32; i++)
			{
				block[i] = -1;
			}
			LRU(size, p_t, block);
		}
		{
			for (int i = 0; i < 30000; i++)
			{
				p_t[i].pagenum = temp[i];
				p_t[i].visit = 0;
				p_t[i].modify = 0;
				p_t[i].blocknum = -1;
				p_t[i].presence = 0;
			}
			for (int i = 0; i < 32; i++)
			{
				block[i] = -1;
			}
			clock(size, p_t, block);
		}
		{
			for (int i = 0; i < 30000; i++)
			{
				p_t[i].pagenum = temp[i];
				p_t[i].visit = 0;
				p_t[i].modify = 0;
				p_t[i].blocknum = -1;
				p_t[i].presence = 0;
			}
			for (int i = 0; i < 32; i++)
			{
				block[i] = -1;
			}
			PBA(size, p_t, block, freeblock);
		}
	}
	return 0;
}