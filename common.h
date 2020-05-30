#pragma once

/* 协议号 */
#define	PROTO_IP	0x0800
#define	PROTO_TCP	6
#define	PROTO_UDP	17


typedef unsigned long ULONG;
typedef unsigned char UCHAR;
typedef unsigned int UINT;
typedef unsigned short USHORT;

static const ULONG INDEX_INVALID = 0xffffffff;

/* 时间 */
static const long SEC2MSEC = 1000;		//1秒等于1000毫秒
static const long MSEC2USEC = 1000;		//1毫秒等于1000微秒
static const long SEC2USEC = SEC2MSEC * MSEC2USEC;
//超时扫描参数
static const long CACHE_TIMEOUT = 1;				// 缓存项超时间隔，单位：秒
static const long SCAN_INTERVAL = 100 * 1000;		//超时扫描周期，单位：微秒
static const long FLOW_TIMEOUT = 10;				//流超时间隔，单位：秒

typedef struct Time {
	long sec;	//秒
	long usec;	//微秒

	Time(long s = 0, long us = 0) {
		sec = s;
		usec = us;
	}

	inline Time operator=(const Time& t) {
		sec = t.sec;
		usec = t.usec;
		return *this;
	}

	inline bool operator!=(const Time& t) {
		return sec != t.sec || usec != t.usec;
	}

	inline bool operator<(const Time& t) {
		return sec < t.sec || sec == t.sec && usec < t.usec;
	}

	inline Time operator+(const Time& t)
	{
		Time ret;
		ret.sec = sec + t.sec;
		ret.usec = usec + t.usec;
		if (ret.usec >= SEC2USEC)
		{
			ret.usec -= SEC2USEC;
			ret.sec++;
		}

		return ret;
	}

	inline Time operator-(const Time& t)
	{
		Time ret;
		if (sec < t.sec || sec == t.sec && usec < t.usec)
		{
			ret.sec = t.sec - sec;
			ret.usec = t.usec - usec;
		}
		else
		{
			ret.sec = sec - t.sec;
			ret.usec = usec - t.usec;
		}

		if (ret.usec < 0)
		{
			ret.sec--;
			ret.usec += SEC2USEC;
		}
		return ret;
	}
	inline void Reset() {
		sec = usec = 0;
	}
} Time, * PTime;

/* 统一数据类型 */
typedef unsigned char	uint8_t;
typedef signed   short	int16_t;
typedef unsigned short	uint16_t;
typedef signed   int	int32_t;
typedef unsigned int	uint32_t;

/* 协议数据结构 */
typedef struct    // 14 bytes
{
	uint8_t  dst_host[6];
	uint8_t  src_host[6];
	uint16_t ether_type;
}EtherHeader, * PEtherHeader;


/* The IPv4 packet is like this. Took from RFC791.
0                   1                   2                   3
0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|Version|  IHL  |Type of Service|          Total Length         |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|         Identification        |Flags|      Fragment Offset    |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|  Time to Live |    Protocol   |         Header Checksum       |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                       Source Address                          |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                    Destination Address                        |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                    Options                    |    Padding    |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                             data                              |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/

typedef struct    // 20 bytes
{
	uint8_t		ver_ihl;    	// Version (4 bits) + Internet header length (4 bits)
	uint8_t		tos;	    	// Type of service 
	uint16_t	total_len;	    // Total length 
	uint16_t	ident;	     	// Identification
	uint16_t	flag_frag;  	// Flags (3 bits) + Fragment offset (13 bits)
	uint8_t		ttl;	    	// Time to live
	uint8_t		protocol;		// Protocol
	uint16_t	checksum;		// Header checksum
	uint32_t	srcIP;			// Source address
	uint32_t	dstIP;			// Destination address
}IpHeader, * PIpHeader;

/* The TCP segment is like this. Took from RFC793.
0                   1                   2                   3
0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|          Source Port          |       Destination Port        |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                        Sequence Number                        |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                    Acknowledgment Number                      |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|  Data |           |U|A|P|R|S|F|                               |
| Offset| Reserved  |R|C|S|S|Y|I|            Window             |
|       |           |G|K|H|T|N|N|                               |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|           Checksum            |         Urgent Pointer        |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                    Options                    |    Padding    |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                             data                              |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/

typedef struct    // 20 bytes
{
	uint16_t   src_port;           // source port 
	uint16_t   dst_port;           // destination port
	uint32_t   seq_num;            // sequence number
	uint32_t   ack_num;            // acknowledgement number
	uint8_t    data_off;           // data offset(4 bits) + reserved(4 bits)
	uint8_t    flags;              // reserved(2 bits) + control field
	uint16_t   window;             // window 
	uint16_t   checksum;           // checksum 
	uint16_t   urg_ptr;            // urgent pointer
} TcpHeader, * PTcpHeader;

/* The UDP datagram is like this. Took from RFC768.
0                   1                   2                   3
0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|          Source Port          |       Destination Port        |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|            Length             |           Checksum            |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                             data                              |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/

typedef struct    // 8 bytes
{
	uint16_t   src_port;           // source port 
	uint16_t   dst_port;           // destination port
	uint16_t   length;				// length 
	uint16_t   checksum;           // checksum 
} UdpHeader, * PUdpHeader;


/* 端点标识符(Endpoint Identifier) */
typedef struct EndID
{
public:
	UINT	ip;		//IP地址
	USHORT	port;	//端口号

	EndID() : ip(0), port(0)
	{
	}

	inline EndID(const EndID& rhs) {
		this->ip = rhs.ip;
		this->port = rhs.port;
	}

	inline EndID& operator=(const EndID& rhs) {
		this->ip = rhs.ip;
		this->port = rhs.port;
		return *this;
	}

	//重载等于操作符
	inline bool operator==(const EndID& rhs)
	{
		return ((ip == rhs.ip) && (port == rhs.port));
	}

	inline bool operator!=(const EndID& rhs) {
		return ((ip != rhs.ip) || (port != rhs.port));
	}

	//重载小于操作符
	inline bool operator<(const EndID& rhs)
	{
		if (ip < rhs.ip)
		{
			return true;
		}
		else if (ip == rhs.ip)
		{
			if (port < rhs.port)
			{
				return true;
			}
		}
		return false;
	}

	//重载大于操作符
	inline bool operator>(const EndID& rhs)
	{
		if (ip > rhs.ip)
		{
			return true;
		}
		else if (ip == rhs.ip)
		{
			if (port > rhs.port)
			{
				return true;
			}
		}
		return false;
	}

	//重载与操作符
	inline EndID& operator&(const EndID& rhs)
	{
		ip = ip & rhs.ip;
		port = port & rhs.port;
		return *this;
	}

	//判断是否为空
	inline bool isEmpty() {
		return ip == 0 && port == 0;
	}
	
	//重置当前项
	inline void reset() {
		ip = 0;
		port = 0;
	}

	~EndID()
	{
	}

}EndID, * PEndID;

/*数据包信息 */
typedef struct Packet
{
	Time		time;			//时间戳
	EndID		src, dst;		//源端，目的端
	UCHAR		proto;			//IP首部中的协议字段

	Packet() : proto(0)
	{
	}
}Packet, * PPacket;

/* 流标识符(Flow Identifier) */
static const USHORT FID_LEN = 2 * (sizeof(UINT) + sizeof(USHORT)) + sizeof(UCHAR);

typedef struct FlowID
{
	UCHAR	proto;	//协议类型：TCP, UDP
	EndID	src, dst;	//源端，目的端

	FlowID() : proto(0)
	{
	}

	//拷贝构造函数
	inline FlowID(const FlowID & rhs) {
		this->proto = rhs.proto;
		this->src = rhs.src;
		this->dst = rhs.dst;
	}

	inline FlowID& operator=(const FlowID& rhs) {
		this->proto = rhs.proto;
		this->src = rhs.src;
		this->dst = rhs.dst;
		return *this;
	}

	//重载等于操作符
	inline bool operator==(const FlowID& rhs)
	{
		return (proto == rhs.proto && src == rhs.src && dst == rhs.dst);
	}

	inline bool operator!=(const FlowID& rhs) {
		return (proto != rhs.proto || src != rhs.src || dst != rhs.dst);
	}

	//重载与操作符
	inline FlowID& operator&(const FlowID& rhs)
	{
		proto = proto & rhs.proto;
		src = src & rhs.src;
		dst = dst & rhs.dst;
		return *this;
	}

	inline bool isEmpty() {
		return src.isEmpty() && dst.isEmpty() && proto == 0;
	}

	inline void reset() {
		src.reset();
		dst.reset();
		proto = 0;
	}

	//将流关键字转化为字节流
	void ToData(UCHAR* buf)
	{
		//填充数据块
		*buf = proto;
		buf += sizeof(UCHAR);
		*((UINT*)buf) = src.ip;
		buf += sizeof(UINT);
		*((USHORT*)buf) = src.port;
		buf += sizeof(USHORT);
		*((UINT*)buf) = dst.ip;
		buf += sizeof(UINT);
		*((USHORT*)buf) = dst.port;
		buf += sizeof(USHORT);
	}
}FlowID, * PFlowID;

//带有节点的链表遍历
#define CIRCULAR_LINKEDLIST_FOR_EACH(PCUR,PHEAD) \
	for ((PCUR) = (PHEAD)->next;(PCUR) != (PHEAD);(PCUR)=(PCUR)->next) \

//双向链表中，删除某个结点
#define CIRCULAR_LINKEDLIST_DEL_NODE(PNODE) \
	(PNODE)->prior->next = (PNODE)->next;	\
	(PNODE)->next->prior = (PNODE)->prior;	\
	(PNODE)->prior = (PNODE)->next = NULL;	\

//双向链表中，在cur结点前增加某个结点  
#define CIRCULAR_LINKEDLIST_INSERT_NODE(PCUR,PNODE) \
	(PNODE)->next = (PCUR);	\
	(PNODE)->prior = (PCUR)->prior;	\
	(PCUR)->prior->next = (PNODE);	\
	(PCUR)->prior = (PNODE);	\

//带头结点单向不循环链表遍历
#define SINGLY_LINKED_LIST_FOR_EACH(PCUR,PHEAD) \
	for ((PCUR) = (PHEAD) -> next; (PCUR) != (NULL);(PCUR)=(PCUR)->next) \