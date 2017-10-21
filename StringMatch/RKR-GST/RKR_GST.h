#pragma once

#include<string>
#include<vector>
#include <queue>
using namespace std;

struct Token
{
	Token ( int pos = 0, int len = 0, bool mark = false )
	{
		hasMarked = mark;
		start = pos;
		length = len;
	}

	int start;//T match start pos
	int length;//match len
	bool hasMarked;
};

class Title
{
public:
	Title ( int tStart = 0, int pStart = 0, int len = 0 )
	{
		this->tStart = tStart;
		this->pStart = pStart;
		this->len = len;
	}
	int tStart;
	int pStart;
	int len;
};

bool operator<( const Title &t1, const Title &t2 )
{
	return t1.len<t2.len;
}


class RKR_GST
{
public:
	RKR_GST ( string  &t, string &p,vector<Title> &matches,int minLength );

	~RKR_GST ();

	vector<bool>mark_1;//对t1每一个字符标注是否已标记
	vector<bool> mark_2;//对t2每一个字符标注是否已标记
private:
	int searchLen;//搜索长度
	int minLength;//最小匹配长度
	string  t1;
	string  t2;

	vector<Token>tTable;//用于记录t1中已标记块和未标记块 
	vector<Token>pTable;//用于记录t2中已标记块和未标记块
	priority_queue<Title>titles;//用于保存一次以s搜索找出的匹配记录 根据匹配长度len由大到小排序

	int scanpattern ( );
	int getHashValue ( string &T, int p, int len, int d = 2, int q = 32 );//p 起始位置  len 用于hash的长度    d 所用进制  q 哈希表格数    折叠法计算hash值
	void markstrings ( vector<Title> &matches );
};

RKR_GST::RKR_GST ( string &t1, string &t2, vector<Title> &matches, int minLength )
{
	this->t1 = t1;
	this->t2 = t2;
	this->mark_1.assign ( t1.length (), false );
	this->mark_2.assign ( t2.length (), false );
	this->minLength = minLength;
	this->searchLen = minLength;//以minLength 为初始搜索长度
	this->tTable.push_back ( Token ( 0, t1.length() ) );
	this->pTable.push_back ( Token ( 0, t2.length() ) );

	bool stop = false;
	do{
		int max_match_length = scanpattern ( );//以长S搜索时的最大长度
		if ( max_match_length > 2 * searchLen )
		{
			searchLen = max_match_length;   //最大匹配子串被搜索多于2次  造成大量重复  故以更大的S搜索
		}
		else
		{
			markstrings ( matches );     
			if ( searchLen > 2 * minLength ) searchLen = searchLen / 2;
			else if ( searchLen > minLength ) searchLen = minLength;
			else stop = true;
		}
	} while ( !stop );
}

RKR_GST::~RKR_GST ()
{
}

int RKR_GST::getHashValue ( string &T, int p, int len, int d, int q )
{
	static int pre_p = 0;
	static int sum = 0;
	int h = 0;
	if( p == pre_p + 1)
	{
		sum = sum*d + T.at(p+len-1) - T.at(pre_p)*pow(d,len);	
	}
	else
	{
		pre_p = p;
		sum = 0;
		for ( int i = p; i < p + len; i++ )
		{
			sum = d*sum + T.at ( i );
		}
	}	
	h = sum % q;
	return h;
}

//在文本块记录表中 查找自pos个块起的首个未标记块
int find_first_unmarked_token ( vector<Token>& t, int pos )
{
	for ( int i = pos; i < t.size (); i++ )
	{
		if ( !(t[i].hasMarked )) return i;
	}
}

int RKR_GST::scanpattern (  )
{
	int max_match_length;
	vector<int> hash_table[32];
	//文本t1
	for ( int i = find_first_unmarked_token ( tTable, 0 ); i < tTable.size (); i = find_first_unmarked_token ( tTable, i + 1 ) )
	{
		int end = tTable[i].length - searchLen + 1;
		for(int pos = 0 ; pos < end; pos++)//当子串长度大于搜索长度时 将该未标记块中所有长S的子串映射到hash表 
		{	
			int h = getHashValue ( t1, tTable[i].start + pos, searchLen );
			hash_table[h].insert ( hash_table[h].end (), tTable[i].start + pos );
		}
	}
	//文本t2
	for ( int i = find_first_unmarked_token ( pTable, 0 ); i < pTable.size (); i = find_first_unmarked_token ( pTable, i + 1 ) )
	{
		int end = pTable[i].length - searchLen + 1;
		for(int pos = 0 ; pos < end; pos++)
		{
			int pStart = pTable[i].start + pos;
			int h = getHashValue ( t2, pStart, searchLen );
			for ( int j = 0;j < hash_table[h].size(); j++ )//查找t中具有与p相同hash值的最大子串  默认前S个字符相同 减少计算
			{
				int tStart = hash_table[h][j];
				int k = searchLen;
				while ( tStart + k < t1.length ()
					&& pStart + k < t2.length()
					&& t2[pStart + k] == t1[tStart + k]
					&& !mark_1[tStart + k]
					&& !mark_2[pStart + k] )
					k++;
				if ( k > 2 * searchLen ) return k;
				else
				{
					max_match_length = k;
					titles.push ( Title ( tStart, pStart, k ) );//纪录所有>=S的匹配块
				}
			}
		}
	}

	return max_match_length;
}

//通过查找块范围内的字符是否已记录 检测是否与之前的块重复
bool isOccluded (vector<bool> &mark_1,vector<bool> &mark_2, Title &title )
{
	for ( int i = title.tStart; i < title.tStart + title.len; i++ )
	{
		if ( mark_1[i] )return true;
	}
	for ( int i = title.pStart; i < title.pStart + title.len; i++ )
	{
		if ( mark_2[i] )return true;
	}
	return false;
}

bool equal ( string &t1, int tStart, string &t2, int pStart,int searchLen )
{
	for ( int i = 0; i < searchLen; i++ )
	{
		if ( t1[tStart + i] != t2[pStart + i] ) return false;
	}
	return true;
}

//更新文本记录块表  i 是要更新的块
void updateTable ( vector<Token>& table, int i, int start, int len )
{
	if ( table[i].start < start )//块中的后一部分被标记
	{
		if ( table[i].length > start - table[i].start + len )//块被切分为3部分  --未标记 标记  未标记
		{
			Token t ( start + len, table[i].length + table[i].start - start - len );
			table.insert (  table.begin()+i+1,t );
		}
		Token t ( start, len, true );
		table[i].length = start - table[i].start;
		table.insert ( table.begin () + i + 1, t );
	}
	else if ( table[i].length < len )//块中的前一部分被标记 块被切分为2部分  --标记  未标记
	{
		Token t ( start + len, table[i].length + table[i].start - start - len );
		table.insert ( table.begin () + i + 1, t );
		table[i].length = len;
		table[i].hasMarked = true;
	}
	else//整个块被标记
	{
		table[i].hasMarked = true;
	}
}

//找出字符起始位置为pos 的子串对应的块  二分查找
int findToken ( vector<Token>& table, int pos )
{
	int head = 0, tail = table.size (), mid;
	do{
		mid = ( head + tail ) / 2;
		if ( table[mid].start <= pos && pos < table[mid].start + table[mid].length ) return mid;
		else if ( pos < table[mid].start ) tail = mid - 1;
		else head = mid + 1;
	} while ( head <= tail );
	return -1;
}


//标记操作 要 更新字符标记表  和 文本记录块
void mark ( vector<bool> &symble, vector<Token>& table, int start, int len )
{
	for ( int i = start; i < start + len; i++ )
		symble[i] = true;
	int i = findToken ( table, start );
	updateTable ( table, i, start, len );
}

void RKR_GST::markstrings ( vector<Title> &matches )
{
	//标记以长S的搜索长度找出的匹配子串
	while (!titles.empty()&& titles.top ().len >= searchLen )
	{
		Title t = titles.top ();
		titles.pop ();
	
		if ( !isOccluded (mark_1,mark_2, t ))
		{
			if ( equal ( t1, t.tStart, t2, t.pStart,searchLen ) )//前S个字符确实匹配时 两个子串才完全匹配  标记 
			{
				mark ( mark_1, tTable, t.tStart,t.len );
				mark ( mark_2, pTable, t.pStart, t.len );
				matches.push_back ( t );
			}
		}
	}
	while ( !titles.empty () ) titles.pop ();
}