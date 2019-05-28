#pragma warning(disable:4996)
#include<stdio.h>
#include<stdlib.h>

struct Page
{
	int pagenum;//页号
	int blocknum;//所存储的块号
	int presence = 0;//存在位，初始为0表示不存在
	int modify = 0;//修改位，初始位0，表示未被修改
	int visit = 0;//访问位，初始值未0表示未被访问
};
//Page page[64];//模拟页面序列
//int BLOCK[32] = {-1};//维护的物理块,初始值为-1表示块内为空，值为块内存在的页面
int free_block[6];//空闲块，用于PBA算法
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
				if (BLOCK[i] == -1)//找到空闲块直接存储
				{
					BLOCK[i] = page[num].pagenum;
					page[num].blocknum = i;
					num++;
					tag = 1;
					i = 32;
				}
			}
			if (tag == 0)//未找到空闲块
			{
				int b = 0;
				int change = -1;//决定替换内容的页位置
				int page_c;//决定替换的页号
				while (b < 32)
				{
					int temp = find_optimal(BLOCK[b], num, page);
					if (temp > change)//不断迭代找到最久未使用的页面号
					{
						change = temp;
						page_c = page[change].pagenum;
					}
					if (temp == -1)
					{
						page_c = BLOCK[b];
						b = 64;//跳出循环，已找到以后不会使用的页面
					}
					b++;
				}
				for (int i = 0; i < 32; i++)//找到对应物理内存中要替换的页
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
	printf("最佳置换算法：*****缺页次数：%d，缺页率：%.3f\n", deletion, rate);
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
				if (BLOCK[i] == -1)//找到空闲块直接存储
				{
					BLOCK[i] = page[num].pagenum;
					page[num].blocknum = i;
					num++;
					tag = 1;
					i = 32;
				}
			}
			if (tag == 0)//未找到空闲块
			{
				int b = 0;
				int change = 40000;//决定替换内容的页位置
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
				for (int i = 0; i < 32; i++)//找到对应物理内存中要替换的页
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
	printf("先进先出置换算法：******缺页次数：%d，缺页率：%.3f\n", deletion, rate);
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
				if (BLOCK[i] == -1)//找到空闲块直接存储
				{
					BLOCK[i] = page[num].pagenum;
					page[num].blocknum = i;
					num++;
					tag = 1;
					i = 32;
				}
			}
			if (tag == 0)//未找到空闲块
			{
				int b = 0;
				int change = 40000;//决定替换内容的页位置
				while (b < 32)
				{
					for (int j = num; j >= 0; j--)//从当前位置向前找最近一次使用的记录
					{
						if (page[j].pagenum == BLOCK[b])
						{
							if (j < change)
								change = j;
							j = -1;//第一次找到跳出循环
						}
					}
					b++;
				}
				for (int i = 0; i < 32; i++)//找到对应物理内存中要替换的页
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
	printf("LRU置换算法：*****缺页次数：%d，缺页率：%.3f\n", deletion, rate);
}

void clock(int size, Page page[30000], int BLOCK[32])
{
	int num = 0;
	int deletion = 0;
	while (num < size)
	{
		if (find_all(page[num].pagenum, BLOCK))//在现有的物理内存中找到页面修改其访问位为1
		{
			page[num].visit = 1;
			num++;
		}
		else
		{
			int tag = 0;
			for (int i = 0; i < 32; i++)
			{
				if (BLOCK[i] == -1)//找到空闲块直接存储
				{
					BLOCK[i] = page[num].pagenum;
					page[num].blocknum = i;
					num++;
					tag = 1;
					i = 32;
				}
			}
			if (tag == 0)//未找到空闲块
			{
				int b = 0;
				int flag = 0;//判断是否找到替换页面，初始值为0表示未找到
				int change = 40000;//决定替换页的物理位置
				while (!flag)//直到找到为止
				{
					for (int i = 0; i < 32 && !flag; i++)
					{
						int place = get_pageplace(BLOCK[i], page, num);
						if (page[place].visit == 0 && page[place].modify == 0)
						{
							if (i < change)//最开始找到的为替换位置
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
								page[place2].visit = 0;//将所有访问到的页面访问位都置0
								if (i < change)//最开始找的的为替换位置
								{
									change = i;
									flag = 1;
								}
							}
						}
					}
				}
				BLOCK[change] = page[num].pagenum;//直接在替换位置替换
				deletion++;
				num++;
			}
		}
	}
	double rate = (double(deletion) / double(size));
	printf("改进clock算法：****缺页次数：%d，缺页率：%.3f\n", deletion, rate);
}

void PBA(int size, Page page[30000], int BLOCK[32], int freeblock[2])//freeblock为空闲块，用于替换换下来的块
{
	freeblock[0] = -1;
	freeblock[1] = -1;//初始化为-1表示没有存储内容
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
				if (BLOCK[i] == -1)//找到空闲块直接存储
				{
					BLOCK[i] = page[num].pagenum;
					page[num].blocknum = i;
					tag = 1;
					i = 32;
					num++;
				}
			}
			if (tag == 0)//未找到空闲块
			{
				int b = 0;
				int change = 40000;//决定替换内容的页位置
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
				for (int i = 0; i < 32; i++)//找到对应物理内存中要替换的页
				{
					int exit = 0;
					if (BLOCK[i] == page[change].pagenum)
					{
						for (int j = 0; j < 2; j++)//从存储置换下来页面的空闲块内寻找是否存在要置换的页面
						{
							if (freeblock[j] == page[change].pagenum)//如果存在，直接置换块，将换下来的块加入空闲块的队列
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
	printf("PBA算法：****缺页次数：%d，缺页率：%.3f\n", deletion, rate);
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
		//printf("如果继续加大页面访问序列串的长度请输入：1,否则输入：0\n");
		//scanf("%d", &flag);
	}
	return place;
}
int main()
{
	int temp[30000];
	int freeblock[2];
	int size;
	Page p_t[30000];//模拟页面序列
	int block[32];//维护的物理块,初始值为-1表示块内为空，值为块内存在的页面、
	for (int i = 0; i < 32; i++)
	{
		block[i] = -1;
	}
	char flag = 'b';
	size = product(temp, 200, 0, 15, 1000, 0.5);
	while (flag != 'e')
	{
		printf("继续输入：b,退出输入：e\n");
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