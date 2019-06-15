#include "FileSystem.h"
#pragma warning(disable:6031)
#pragma warning(disable:4996)

void read_block(int i, char* p) {
	for (int j = 0; j < B; j++) {
		p[j] = ldisk[i][j];
	}
}

void write_block(int i, char* p) {
	for (int j = 0; j < B; j++) {
		if (p[j] > 0)
			ldisk[i][j] = p[j];
		else
			ldisk[i][j] = '0';
	}
}

void write_to_file() {
	int i = 0, j = 0;
	//д��λͼ
	for (i = 0; i < L; i++)
		ldisk[0][i] = bmp[i] + '0';
	//д���ļ�������
	for (j = 0; j < fdcpsize; j++) {
		*(ldisk[1] + 4 * j) = fdcp[j].len + '0';
		*(ldisk[1] + 4 * j + 1) = fdcp[j].fpos[0] + '0';
		*(ldisk[1] + 4 * j + 2) = fdcp[j].fpos[1] + '0';
		*(ldisk[1] + 4 * j + 3) = fdcp[j].fpos[2] + '0';
	}
	//д��Ŀ¼
	int p = 0;
	for (i = 0; i < findex.num; i++) {
		*(ldisk[3] + p++) = findex.index[i].fdcpnum + '0';
		for (j = 0; j < strlen(findex.index[i].fname); j++, p++)
			*(ldisk[3] + p) = findex.index[i].fname[j];
		*(ldisk[3] + p++) = '!';
	}
	for (; p < 20; p++) {
		*(ldisk[3] + p) = '0';
	}
	FILE* fp = freopen("ldisk.txt", "w", stdout);
	for (i = 0; i < L; i++) {
		for (j = 0; j < B; j++) {
			printf("%c ", ldisk[i][j]);
		}
		printf("\n");
	}
	printf("* * * * * * * * * * * *");
	fclose(stdout);
	fp = freopen("CON", "w", stdout);
}

int read_from_file() {

	FILE* fp = freopen("ldisk.txt", "r", stdin);
	fseek(fp, 0, SEEK_END);
	int value = ftell(fp);
	//�ļ�Ϊ��
	if (value == 0) {
		fclose(stdin);
		freopen("CON", "r", stdin);
		return -1;
	}
	//�ļ���
	if (value < 243) {
		fclose(stdin);
		freopen("CON", "r", stdin);
		return -2;
	}
	fseek(fp, 0, SEEK_SET);
	for (int i = 0; i < L; i++) {
		for (int j = 0; j < B - 1; j++) {
			scanf("%c ", &ldisk[i][j]);
		}
		scanf("%c\n", &ldisk[i][B - 1]);
	}
	fclose(stdin);
	freopen("CON", "r", stdin);
	return 0;
}

void init() {
	int i = 0, j = 0, value = 0;
	//���ļ�����
	value = read_from_file();
	switch (value)
	{
	case -1: printf("Ӳ��Ϊ��\n");
	case -2: {
		if (value == -2)
			printf("Ӳ���ļ���\n");
		//һ�б����ÿ�
		for (i = 0; i < K + 2; i++)
			bmp[i] = 1;
		for (i = 0; i < fdcpsize; i++) {
			fdcp[i].len = 0;
			for (j = 0; j < 3; j++)
				fdcp[i].fpos[j] = 0;
		}
		findex.index = NULL;
		findex.size = 0;
		findex.num = 0;
		for (int i = 0; i < 5; i++) {
			oft[i].buff = NULL;
			oft[i].rwpos = NULL;
			oft[i].fpos = 0;
		}
		break;
	}
	case 0: {
		//��ʼ��λͼ
		for (i = 0; i < B; i++) {
			bmp[i] = (int)ldisk[0][i] - '0';
		}
		//��ʼ���ļ�������
		for (i = 0; i < fdcpsize; i++) {
			fdcp[i].len = (int)(*(ldisk[1] + i * 4) - '0');
			for (int j = 0; j < 3; j++) {
				fdcp[i].fpos[j] = (int) * (ldisk[1] + j + 1 + i * 4) - '0';
			}
		}
		//��ʼ��Ŀ¼
		int p = 0, count = 0;
		findex.size = fdcp[0].len;
		for (i = 0; count < fdcp[0].len - 1; i++) {
			int num = 0;
			j = 0;
			findex.index = (Index*)realloc(findex.index, ++findex.num * sizeof(Index));
			while (*(ldisk[3] + p) != '!') {
				if (num == 0) {
					findex.index[i].fdcpnum = *(ldisk[3] + p) - '0';
				}
				else
				{
					tmp[i][j] = *(ldisk[3] + p);
					j++;
				}
				p++;
				num++;
			}
			tmp[i][j] = '\0';
			findex.index[i].fname = tmp[i];
			p++; num++;
			count += num;
		}
	}
	}
}

int create(char* filename) {
	int i = 0, startpos = 0;
	//Ѱ�ҿ�������
	for (i = 1; i < fdcpsize; i++) {
		if (fdcp[i].fpos[2] == 0) {
			startpos = i;
			fdcp[i].fpos[2] = 1;
			break;
		}
	}
	//���޿�������������-1
	if (startpos == 0)
		return -1;
	//����Ŀ¼��
	//��Ŀ¼���䲻�ɹ�������-2
	findex.index = (Index*)realloc(findex.index, (findex.num + 1) * sizeof(Index));
	if (findex.index == NULL)
		return -2;
	else findex.num++;
	//����ռ�
	int have = 0;
	for (i = 0; i < L; i++) {
		//ɨ��λͼ��Ѱ�ҿ��̷�
		if (bmp[i] == 0) {
			fdcp[startpos].fpos[0] = i;
			fdcp[startpos].fpos[1] = 0;
			bmp[i] = 1;
			have = 1;
			break;
		}
	}
	//û�п��̷�������-3
	if (have == 0)
		return -3;
	//������Ŀ¼
	fdcp[0].fpos[2] = 1;
	findex.index[findex.num - 1].fdcpnum = startpos;
	findex.size += strlen(filename) + 2;
	fdcp[0].len = findex.size;
	findex.index[findex.num - 1].fname = (char*)malloc(strlen(filename) * sizeof(char));
	strcpy(findex.index[findex.num - 1].fname, filename);
	return 0;
}

int destroy(char* filename) {
	int status = 0, fdcpnum = 0, i = 0, indexnum = 0;//fdcpnum�ļ���������ţ�indexnumĿ¼��ţ�status�Ƿ��ҵ��ļ���ʶ
	for (i = 0; i < findex.num; i++) {
		//Ѱ�Ҹ��ļ����ļ����ַ���ƥ�䣬��ȡ�ļ����������
		if (strcmp(findex.index[i].fname, filename) == 0) {
			status = 1;
			fdcpnum = findex.index[i].fdcpnum;
			indexnum = i;
		}
	}
	//û�и��ļ�������-1
	if (status == 0)
		return -1;
	//ɾ��λͼ���ļ����ļ�������
	bmp[fdcp[fdcpnum].fpos[0]] = 0;
	for (i = 0; i < fdcp[fdcpnum].len; i++) {
		ldisk[fdcp[fdcpnum].fpos[0]][fdcp[fdcpnum].fpos[1] + i] = '0';
	}
	fdcp[fdcpnum].len = 0;
	for (i = 0; i < 3; i++)
		fdcp[fdcpnum].fpos[i] = 0;
	//ɾ��Ŀ¼
	int deslen = 0;//��Ҫɾ����Ŀ¼����
	findex.index[indexnum].fdcpnum = 0;
	deslen = strlen(findex.index[indexnum].fname);
	findex.index[indexnum].fname = NULL;
	findex.size -= deslen + 2;
	fdcp[0].len = findex.size;
	//����Ŀ¼��ǰ��
	for (i = indexnum; i < findex.num; i++) {
		findex.index[i] = findex.index[i + 1];
	}
	findex.num--;
	if (findex.num == 0)
		fdcp[0].fpos[2] = 0;
	return 0;
}

int open(char* filename) {
	//fdcpnum�ļ���������ţ�indexnumĿ¼��ţ�status�Ƿ��ҵ��ļ���ʶ��oftnum���ļ���
	int status = 0, fdcpnum = 0, i = 0, indexnum = 0, oftnum = -1;
	for (i = 0; i < findex.num; i++) {
		//Ѱ�Ҹ��ļ����ļ����ַ���ƥ�䣬��ȡ�ļ����������
		if (strcmp(findex.index[i].fname, filename) == 0) {
			status = 1;
			fdcpnum = findex.index[i].fdcpnum;
			indexnum = i;
			break;
		}
	}
	//û�и��ļ�������-1
	if (status == 0)
		return -1;
	for (i = 0; i < 5; i++) {
		//�ļ��Ѿ��򿪣�����-2
		if (oft[i].fpos == fdcpnum) {
			return -2;
		}
		if (oft[i].fpos == 0) {
			oftnum = i;
			break;
		}
	}
	//���ļ�����������-3
	if (oftnum == -1)
		return -3;
	oft[oftnum].buff = (char*)malloc(B * sizeof(char));
	oft[oftnum].fpos = fdcpnum;
	oft[oftnum].rwpos = 0;
	//�����ļ��̿�
	read_block(fdcp[fdcpnum].fpos[0], oft[oftnum].buff);
	return oftnum;
}

int close(int index) {
	//���ļ�δ�򿪣�����-1
	if (oft[index].fpos == 0)
		return -1;
	//������д���ļ�
	if (oft[index].buff != NULL) {
		int fpos = fdcp[oft[index].fpos].fpos[0];
		write_block(fpos, oft[index].buff);
	}
	//�ͷű�Ŀ
	oft[index].fpos = 0;
	oft[index].rwpos = 0;
	oft[index].buff = NULL;
	return 0;
}

int read(int index, char* mem_area, int count) {
	//���ļ�δ�򿪣�����-1
	if (oft[index].fpos == 0)
		return -1;
	int i = 0;
	//�����ļ��������ҵ��ļ�λ��
	int ds = fdcp[oft[index].fpos].fpos[0];//���̿��
	int sp = fdcp[oft[index].fpos].fpos[1] + oft[index].rwpos;//����ƫ��
	int filelen = fdcp[oft[index].fpos].len;//�ļ�����
	if (oft[index].buff == NULL)
		oft[index].buff = (char*)malloc(B * sizeof(char));
	while (i < count) {
		read_block(ds++, oft[index].buff);
		for (; i < count && i + sp < B && i + oft[index].rwpos < filelen; i++)
			mem_area[i] = oft[index].buff[i + sp];
		//����count������0
		if (i == count)
			break;
		//�����ļ�β������-2
		if (i + oft[index].rwpos == filelen) {
			mem_area[i] = '\0';
			oft[index].rwpos = filelen;
			return -2;
		}
		sp -= B;
	}
	oft[index].rwpos += i;
	mem_area[i] = '\0';
	return 0;
}

int write(int index, char* mem_area) {
	return write(index, mem_area, strlen(mem_area));
}
int write(int index, char* mem_area, int count) {
	//���ļ�δ�򿪣�����-1
	if (oft[index].fpos == 0)
		return -1;
	int i = 0;
	//�����ļ��������ҵ��ļ�λ��
	int ds = fdcp[oft[index].fpos].fpos[0];//���̿��
	int sp = fdcp[oft[index].fpos].fpos[1] + oft[index].rwpos;//����ƫ��
	int filelen = fdcp[oft[index].fpos].len;//�ļ�����
	while (i < count) {
		read_block(ds++, oft[index].buff);
		for (; i < count && sp < B; i++, sp++) {
			oft[index].buff[sp] = mem_area[i];
			oft[index].rwpos++;
		}
		write_block(ds - 1, oft[index].buff);
		bmp[ds - 1] = 1;
		//д��count���ȣ�����0
		if (i == count)
			break;
		//count���󣬸�����һ���ļ�������-2
		int a = ds - fdcp[oft[index].fpos].fpos[0];
		if (bmp[ds] != 0 && filelen - (a * B) <= 0) {
			fdcp[oft[index].fpos].len = oft[index].rwpos;
			oft[index].buff = NULL;
			return -2;
		}
		//��δд��count�����¶�����һ�����
		sp -= B;
	}
	//�����ļ�����
	if (oft[index].rwpos > filelen) {
		fdcp[oft[index].fpos].len = oft[index].rwpos;
	}
	//��ջ�����
	oft[index].buff = NULL;
	return 0;
}

int lseek(int index, int pos) {
	//���ļ�δ�򿪣�����-1
	if (oft[index].fpos == 0)
		return -1;
	//�����ļ��������ҵ��ļ�λ��
	int sp = fdcp[oft[index].fpos].fpos[1] + oft[index].rwpos;//����ƫ��
	//pos�����ļ���С������-2
	if (pos >= fdcp[oft[index].fpos].len)
		return -2;
	oft[index].rwpos = pos;
	return 0;
}

int directory() {
	int i = 0, j = 0;
	printf("\n�ļ��б� 16281042\n���\t�ļ���\t�ļ���\n");
	for (i = 0; i < findex.num; i++) {
		printf("%d\t%s\t", i + 1, findex.index[i].fname);
		printf("%d\n", fdcp[findex.index[i].fdcpnum].len);
	}
	return 0;
}

int exec(char str[]) {
	int i = 0, j = 0, k = 0, len = strlen(str);
	char com[4][30] = { 0, };
	char rtn[50] = "";
	while (i < len) {
		for (j = 0; str[i] != ' ' && str[i] != '\0'; i++, j++) {
			com[k][j] = str[i];
		}
		i++;
		com[k][j] = '\0';
		k++;
	}
	if (strcmp(com[0], "create") == 0) {
		switch (create(com[1])) {
		case 0:strcpy(rtn, com[1]); strcat(rtn, "�����ɹ�"); break;
		case -1:strcpy(rtn, "�޿���������"); break;
		case -2:strcpy(rtn, "Ŀ¼���䲻�ɹ�"); break;
		case -3:strcpy(rtn, "û�п��̷�"); break;
		default:strcpy(rtn, "δ֪���"); break;
		}
	}
	else if (strcmp(com[0], "destroy") == 0) {
		switch (destroy(com[1])) {
		case 0:strcpy(rtn, com[1]); strcat(rtn, "�򿪳ɹ�"); break;
		case -1:strcpy(rtn, "û�и��ļ�"); break;
		default:strcpy(rtn, "δ֪���"); break;
		}
	}
	else if (strcmp(com[0], "open") == 0) {
		switch (open(com[1])) {
		case 4:case 3:case 2:case 1:
		case 0:strcpy(rtn, com[1]); strcat(rtn, "�򿪳ɹ�"); break;
		case -1:strcpy(rtn, "û�и��ļ�"); break;
		case -2:strcpy(rtn, "�ļ��Ѿ���"); break;
		case -3:strcpy(rtn, "���ļ�������"); break;
		default:strcpy(rtn, "δ֪���"); break;
		}
	}
	else if (strcmp(com[0], "close") == 0) {
		switch (close(com[1][0] - '0')) {
		case 0:strcpy(rtn, com[1]); strcat(rtn, "���ļ��رճɹ�"); break;
		case -1:strcpy(rtn, "�ļ�δ��"); break;
		default:strcpy(rtn, "δ֪���"); break;
		}
	}
	else if (strcmp(com[0], "read") == 0) {
		int count = 0;
		for (i = 0; com[2][i] >= '0'; i++) {
			count = count * 10 + com[2][i] - '0';
		}
		char* p = (char*)malloc(count * sizeof(char));
		switch (read(com[1][0] - '0', p, count)) {
		case 0: {
			strcpy(rtn, com[1]);
			strcat(rtn, "���ļ���ȡ�ɹ�,����Ϊ\n");
			strcat(rtn, p);
			break;
		}
		case -1:strcpy(rtn, "�ļ�δ��"); break;
		case -2:strcpy(rtn, "�����ļ�β,��ȡ��������Ϊ\n"); strcat(rtn, p); break;
		default:strcpy(rtn, "δ֪���"); break;
		}
	}
	else if (strcmp(com[0], "write") == 0) {
		char* p = (char*)malloc((com[2][0] - '0') * sizeof(char));
		int a = 0;
		if (com[3][0] - '0' <= 0)
			a = write(com[1][0] - '0', com[2]);
		else
			a = write(com[1][0] - '0', com[2], com[3][0] - '0');
		switch (a) {
		case 0: strcpy(rtn, com[1]); strcat(rtn, "���ļ�д��ɹ�"); break;
		case -1:strcpy(rtn, "�ļ�δ��"); break;
		case -2:strcpy(rtn, "�������¸��ļ�"); break;
		default:strcpy(rtn, "δ֪���"); break;
		}
	}
	else if (strcmp(com[0], "lseek") == 0) {
		switch (lseek(com[1][0] - '0', com[2][0] - '0')) {
		case 0: strcpy(rtn, com[1]); strcat(rtn, "���ļ��ƶ��ɹ�"); break;
		case -1:strcpy(rtn, "�ļ�δ��"); break;
		case -2:strcpy(rtn, "�����ļ�β"); break;
		default:strcpy(rtn, "δ֪���"); break;
		}
	}
	else if (strcmp(com[0], "directory") == 0) {
		directory();
	}
	else if (strcmp(com[0], "exit") == 0) {
		return -1;
	}
	else
	{
		printf("��֧�ָ�ָ��\n");
		return -2;
	}
	printf("%s\n", rtn);
	return 0;
}

void menu() {
	char com[50];
	int status = 1;
	printf("����ϵͳʵ�� �ļ�ϵͳ 16281042\n");
	printf("create filename\ndestroy filename\nopen filename\nclose index\nread index count\nwrite index string count / write index string\nlseek index pos\ndirectory\n");
	printf("������ָ��Կո�ָ�\n");
	while (status >= 0) {
		fflush(stdin);
		printf("osh>");
		gets_s(com);
		status = exec(com);
	}
}

int main()
{
	int oftnum = 0;
	init();
	menu();
	write_to_file();
	return 0;
}