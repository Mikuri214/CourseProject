// MemDispatch.cpp : 定义控制台应用程序的入口点。
//
#include<iostream>
#include<string>
#include<cstdlib>
#include<ctime>
using namespace std;

#define NUM_OF_INSTRU 320

class Page {
public:
	Page( int page = -1 ) {
		R = W = count = 0;
		this->page = page;
		this->fetchTime = 0;
		pagr = coun; coun++;
	}
	int R;
	int W;
	int count;
	int page;
	long fetchTime;
	static int coun;
	int pagr;
};
int Page::coun = 0;

class PageNode {
public:
	PageNode( PageNode *next = NULL ) {
		this->page = new Page( );
		this->next = next;
	}
	~PageNode( ) { delete page; }
	Page *page;
	PageNode *next;
};

class PageList {
public:
	int size;
	PageNode *head, *tail;
	PageList( int size ) {
		this->size = size;
		head = tail = new PageNode( );

		PageNode *curr = head;
		for ( int i = 1; i < size; i++ ) {
			curr = curr->next = new PageNode( );
		}
	}
	~PageList( )
	{
		PageNode *temp;
		while ( head->next != NULL )
		{
			temp = head;
			head = head->next;
			delete temp;
		}
		delete head;
	}
	PageNode* findPage( int &page ) {
		PageNode *curr = head;
		while ( curr != NULL )
		{
			if ( curr->page->page == page ) return curr;
			curr = curr->next;
		}
		return NULL;
	}
};

class MemDispatchStrategy {
public:
	virtual void visitPage( PageList &memory, PageNode *pageNode, int instruction ) = 0;
	virtual PageNode* selectPage( PageList &memory ) = 0;
	virtual string strategyName( ) = 0;
};

class FIFO :public MemDispatchStrategy {
public:
	PageNode* selectPage( PageList &memory ) {
		//还有空闲的页
		if ( memory.tail->next != NULL || memory.tail->page->fetchTime == 0 ) {
			PageNode* temp = memory.tail;
			if ( memory.tail->next != NULL )memory.tail = memory.tail->next;
			return temp;
		}
		//选择链表头部的页，即最先进入的页，同时移到链表尾部
		PageNode* temp = memory.head;
		memory.head = memory.head->next;
		memory.tail = memory.tail->next = temp;
		temp->next = NULL;
		return temp;
	}
	void visitPage( PageList &memory, PageNode *pageNode, int instruction ) {}
	string strategyName( ) { return "FIFO"; }
};

class LRU :public MemDispatchStrategy {
public:
	PageNode* selectPage( PageList &memory ) {
		//还有空闲的页
		if ( memory.tail->next != NULL || memory.tail->page->fetchTime == 0 ) {
			PageNode* temp = memory.tail;
			if ( memory.tail->next != NULL ) memory.tail = memory.tail->next;
			return temp;
		}

		PageNode* selectPage = memory.head, *temp = selectPage->next;
		while ( temp != NULL ) {
			if ( temp->page->fetchTime < selectPage->page->fetchTime ) selectPage = temp;
			temp = temp->next;
		}
		return selectPage;
	}

	void visitPage( PageList &memory, PageNode *pageNode, int instruction ) {
		pageNode->page->fetchTime = clock( );
	}

	string strategyName( ) { return "LRU"; }
};

class NFU :public MemDispatchStrategy
{
public:
	void fresh( PageList &memory ) {
		PageNode *curr = memory.head;
		int interval;
		while ( curr != NULL ) {
			interval = clock( ) - curr->page->fetchTime;
			curr->page->fetchTime = clock( );
			curr->page->count >> interval;//老化
			curr->page->count |= curr->page->R << 8;
			curr->page->R = 0;
			curr = curr->next;
		}
	}
	void visitPage( PageList &memory, PageNode *pageNode, int instruction ) {
		fresh( memory );
		pageNode->page->R = 1;
		pageNode->page->count++;
	}
	PageNode* selectPage( PageList &memory ) {
		if ( memory.tail->next != NULL || memory.tail->page->fetchTime == 0 ) {
			PageNode* temp = memory.tail;
			if ( memory.tail->next != NULL ) memory.tail = memory.tail->next;
			return temp;
		}
		PageNode *selectPage = memory.head, *temp = selectPage->next;
		while ( temp != NULL ) {
			if ( temp->page->count < selectPage->page->count ) selectPage = temp;
			temp = temp->next;
		}
		return selectPage;
	}
	string strategyName( ) { return "NFU"; }
};

class NRU :public MemDispatchStrategy
{
public:
	void fresh( PageList &memory ) {
		PageNode *curr = memory.head;
		long currTime = clock( );
		while ( curr != NULL && currTime - curr->page->fetchTime > 20 ) {
			curr->page->R = 0;
			curr = curr->next;
		}
	}
	void visitPage( PageList &memory, PageNode *pageNode, int instruction ) {
		pageNode->page->R = 1;
		if ( instruction % 10 == 1 ) pageNode->page->W = 1;//设定个位为1的指令要写回
	}
	PageNode* selectPage( PageList &memory ) {
		fresh( memory );
		if ( memory.tail->next != NULL || memory.tail->page->fetchTime == 0 ) {
			PageNode* temp = memory.tail;
			if ( memory.tail->next != NULL ) memory.tail = memory.tail->next;
			return temp;
		}
		//选择分类号最小的页(以R位权重为2，W位权重为1计算分类
		PageNode *selectPage = memory.head, *temp = selectPage->next;
		while ( temp != NULL ) {
			int value1 = temp->page->R * 2 + temp->page->W;
			int value2 = selectPage->page->R * 2 + selectPage->page->W;
			if ( value1 < value2 )selectPage = temp;
			temp = temp->next;
		}
		return selectPage;
	}
	string strategyName( ) { return "NRU"; }
};

void fetchPage( PageNode* pageNode, int page ) {
	pageNode->page->page = page;
	pageNode->page->R = 0;
	pageNode->page->W = 0;
	pageNode->page->count = 0;
	pageNode->page->fetchTime = clock( );
}

void execute( MemDispatchStrategy *strategy, PageList &memory, int address, int instruction, int &hit, int &miss ) {
	int page = address / 10;
	//	cout << "page" << page;
	PageNode *pageNode = memory.findPage( page );
	if ( pageNode != NULL ) {
		strategy->visitPage( memory, pageNode, address );
		hit++;
		// 	cout << " hit   " << pageNode->page->pagr << endl;
	}
	else {
		PageNode *pageNode = strategy->selectPage( memory );
		fetchPage( pageNode, page );
		strategy->visitPage( memory, pageNode, address );
		miss++;
		//	cout << " miss . select" <<pageNode->page->pagr  << endl;
	}
}


void MemDispatch( MemDispatchStrategy &f, int Physical_mem, int instructions[] ) {
	PageList memory( Physical_mem );
	int instr_pos;

	int hit = 0, miss = 0;
	srand( 10 );
	for ( int i = 0; i < NUM_OF_INSTRU; i += 4 ) {
		instr_pos = rand( ) % ( NUM_OF_INSTRU - 1 );
		//	cout << "line:" << i << "\n" << instr_pos + 1 << "\t";
		execute( &f, memory, instr_pos + 1, instructions[ instr_pos + 1 ], hit, miss );
		instr_pos = rand( ) % instr_pos;
		//	cout << instr_pos << "\t";
		execute( &f, memory, instr_pos, instructions[ instr_pos ], hit, miss );
		//	cout << instr_pos + 1 << "\t";
		execute( &f, memory, instr_pos + 1, instructions[ instr_pos + 1 ], hit, miss );
		instr_pos = rand( ) % ( NUM_OF_INSTRU - 2 - instr_pos ) + instr_pos + 2;
		//	cout << instr_pos << "\t";
		execute( &f, memory, instr_pos, instructions[ instr_pos ], hit, miss );
		//	cout << endl;
	}
	cout << "hit:" << hit << "\t miss:" << miss << "\t hitRate :" << 1 - 1.0* miss / NUM_OF_INSTRU << endl;
}

int main( )
{
	MemDispatchStrategy *strategy[ 4 ];
	FIFO fifo;  strategy[ 0 ] = &fifo;
	NFU nfu;    strategy[ 1 ] = &nfu;
	NRU nru;    strategy[ 2 ] = &nru;
	LRU lru;    strategy[ 3 ] = &lru;

	//初始化指令序列
	srand( time( NULL ) );
	int instructions[ NUM_OF_INSTRU ];
	for ( int i = 0; i < NUM_OF_INSTRU; i += 2 )
	{
		instructions[ i ] = 10 * rand( ) / 65535 * 319 + 1;
		instructions[ i + 1 ] = 10 * rand( ) / 65535 * instructions[ 0 ];
	}


	for ( int j = 4; j <= 32; j++ )
	{
		cout << "--内存大小为 " << j << endl;
		for ( int i = 0; i < 4; i++ )
		{
			cout << strategy[ i ]->strategyName( ) << "\t";
			MemDispatch( *strategy[ i ], j, instructions );
		}
	}

	system( "pause" );
	return 0;
}

