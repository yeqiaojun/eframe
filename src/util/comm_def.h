

#ifndef _COMM_DEF_H_
#define _COMM_DEF_H_

#include <stdint.h>
#include <unistd.h> 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <fcntl.h>
#include <signal.h>
#include <assert.h>
#include <getopt.h>
#include <libgen.h>
#include <netinet/in.h>

#include <sys/stat.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/file.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/epoll.h>
#include <arpa/inet.h>

#include <string>
#include <sstream>
#include <list>
#include <vector>
#include <map>
#include <queue>
#include <unordered_map>
#include <functional>
#include <memory>
#include <type_traits>


//#include "logging.h"

extern "C" 
{    
    #include "lua.h"
    #include "lualib.h"
    #include "lauxlib.h"
};

#include <atomic>

/***系统类型定义 ***/
typedef long int  int64_t;
typedef unsigned long int  uint64_t;
typedef unsigned int uint32_t;
typedef int int32_t;
typedef short int16_t;
typedef unsigned short uint16_t;
typedef signed char int8_t;
typedef unsigned char uint8_t;

typedef std::vector<int32_t> int32_vec;
typedef std::vector<uint64_t> uint64_vec;


/***系统宏定义***/
#define INVALID_SOCKET              -1                        /*无效socket句柄*/
#define IP_LENGTH                   20                        /*IP地址长度*/
#define RECV_BUF_LEN            (16 * 1024)                   /*接收客户端信息的缓冲区*/ 

#define FLUSH() \
    base::GetLogger(INFO)->Flush();\
    base::GetLogger(ERROR)->Flush();


#define PKG_BUFF_SIZE 10240

#define likely(x)       __builtin_expect((x),1)
#define unlikely(x)     __builtin_expect((x),0)

//vs报错处理
#ifdef _WIN32

#define     unlikely(x)   x
#define     likely(x)   x
#define     GOOGLE_PREDICT_BRANCH_NOT_TAKEN(x)  x
#define     GOOGLE_PREDICT_FALSE(x) x
#define     GOOGLE_PREDICT_TRUE(x) x

#endif


#ifndef NDEBUG

#else


#endif // !NDEBUG


uint64_t prime_array[] = {3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41,  43,
47,  53,  59,  61,  67,  71, 73,  79,  83,  89,  97,  101,  103,  107,  109,
113,  127,  131,  137,  139, 149,  151,  157,  163,  167,  173,  179,  181,
191,  193,  197,  199,  211, 223,  227,  229,  233,  239,  241,  251,  257,  
263,  269,  271,  277,  281, 283,  293,  307,  311,  313,  317,  331,  337, 
347,  349,  353,  359,  367, 373,  379,  383,  389,  397,  401,  409,  419,
421,  431,  33,  439,  443, 449,  457,  461,  463,  467,  479,  487,  491,  
499,  503,  509,  521,  523, 541,  547,  557,  563,  569,  571,  577,  587,
593,  599,  601,  607,  613, 617,  619,  631,  641,  643,  647,  653,  659,
661,  673,  677,  683,  691, 701,  709,  719,  911,  1217, 1627,   2179,  
2909,  3881,   6907, 9209,  12281,  16381,  21841,  29123, 38833, 51787,  
69061, 92083,   122777,  163729,  218357,  291143,  388211, 517619,  690163,
999983,  1226959,   1635947,   2181271, 2908361,   3877817,   5170427,  
6893911,   9191891,   12255871, 16341163,  21788233,  29050993,  38734667,
51646229,  68861641, 91815541,  122420729,  163227661,  217636919,  290182597,
386910137, 515880193,  687840301,  917120411,  1222827239,  1610612741, 3221225473ul,
4294967291ul };

constexpr uint64_t upper_prime(int32_t value, int32_t i) {
	return value <= prime_array[i] ?  upper_prime(value, ++i): prime_array[i];
}




/***系统常量参数定义***/

// connclient测试连接数量
const int MAX_CLIENT_CONN = 1;

// connsvr处理最大连接数量
const int MAX_SOCKET_NUM = 200;

const int SOCK_RECV_BUFFER = 512*1024;
const int SOCK_SEND_BUFFER = 512*1024;
const int STR_COMM_LEN = 128;
const int LISTEN_BACKLOG = 512;

const char TCPSVR_PIPE_FILE[] = "tcppipefile";

// 客户端占用共享内存 key
const int CLIENT_SHM_KEY = 1110;

// connsvr->gamesvr消息队列key
const int CS_MQ_SHMKEY = CLIENT_SHM_KEY+1;
// gamesvr->connsvr消息队列key
const int SC_MQ_SHMKEY = CS_MQ_SHMKEY+1;

// gamesvr的共享内存key
const int GAMESVR_SHM_KEY = SC_MQ_SHMKEY+1;

// connsvr<->gamesvr消息队列长度
const int MQ_SIZE = 1024*1024*10;


// CS&SS通讯包包头中长度字段占用的大小
const int PKGHEAD_FIELD_SIZE = sizeof(int); 
// CS&SS通信包最小长度
const int MIN_PKG_LEN = 2*sizeof(int);
// CS&SS通信包最大长度
const int MAX_PKG_LEN = RECV_BUF_LEN;
// CS&SS打包临时缓冲区大小
const int PKG_OPT_BUFFSIZE = RECV_BUF_LEN*2;

// connsvr一次从消息队列中取得的最大消息包数,即一次最多发送给client的消息回包数
const int MAX_SEND_PKGNUM = 512;

//每天秒数
const int DAY_SECS = 24 * 3600;
//每小时秒数
const int HOUR_SECS = 3600;
//每分钟秒数
const int MIN_SECS = 60;


//新的一天开始 时间
#define NEW_DAY_HOUR 5

//每帧最大处理消息数量
const int MAX_LOOP_PER_FRAME = 30;

const std::string empty_str;
extern char file_name_buf[256];

/***系统公共数据结构定义 ***/

// 创建共享内存的模式
enum ENMShmMode
{
    ESM_None = -1,
    ESM_Init = 0,   //全新，即原来没有共享内存新创建
    ESM_Resume = 1,   //恢复，即共享原来已经存在，恢复使用
    ESM_MAX
};

// 消息队列类型
enum MQType
{
    MQ_CS = 1, // connsvr<-->gamesvr
    MQ_SC = 2, // gamesvr<-->connsvr
    MQ_GD = 3, // gamesvr<-->datasvr
    MQ_DG = 4, // datasvr<-->gamesvr
    MQ_GC = 5, // gamesvr<-->chatsvr
    MQ_CG = 6, // chatsvr<-->gamesvr
    MQ_CD = 7, // connsvr<-->dirsvr
    MQ_DC = 8, // dirsvr<-->connsvr                      
    MQ_AGT_SVR = 9, // agentsvr<-->dirsvr
    MQ_AGT_CLI = 10, // gamesvr<-->agentsvr
    MQ_FG = 11,
    MQ_GF = 12,
    MQ_GL = 13, // gamesvr<-->logsvr
    MQ_LG = 14, // logsvr<-->gamesvr
    MQ_MAX
};

// connsvr<-->gamesvr通讯命令字
enum ConnCmd
{
    CONN_NULL  = 0,
    CONN_START = 1,
    CONN_INPROC = 2,
    CONN_STOP = 3
};

// 网络连接出错码定义
enum ENMSocketErr
{
    Err_Succeed            =      0,    /*处理正常*/
    Err_ClientClose        =      -1,  /*客户端关闭*/         
    Err_ClientTimeout    =      -2,  /*客户端超时*/    
    Err_WriteBuffOver    =      -3,  /*Tcp写缓冲区已满*/
    Err_ReadBuffEmpty  =      -4,  /*Tcp读缓冲区为空*/
    Err_PacketError        =      -5, /*客户端发送的包错误*/
    Err_ServerClose     = -6,  /*服务端主动关闭*/
};

// connsvr和客户端通讯的socket结构
struct SocketConnInfo
{
    int socket_fd; // socket句柄
    uint64_t uin; //玩家账号
    int recv_bytes; // 接收的字节数
    char recv_buf[RECV_BUF_LEN];  //接收到的client请求包   
    uint64_t client_ip; // 客户端IP地址
    uint32_t player_idx; // 玩家在gamesvr唯一内存索引
    //uint32_t conn_state; //链接状态
    time_t  create_Time; // socket的创建时间
    time_t  recv_data_time; // 接收到数据包的时间戳
    struct timeval recv_begin_time;//计数周期开始时间
    uint32_t recv_count;//计时器周期内收到的包的数量
};

// connsvr与gamesvr通讯的包头
struct CGPkgHead
{
    void clear()
    {
        player_idx = 0;
        fd = 0;
        state = CONN_NULL;
        client_ip = 0;
        pkg_size = 0;
    }
    int pkg_size; // cs总包长
    int fd; // cs通信socket
    int conn_cmd; // connsvr<-->gamesvr通讯命令字
    uint32_t state;//链接状态
    uint32_t player_idx; // player的唯一索引
    uint64_t client_ip; // 客户端IP地址,以便gamesvr运营时作一些统计用
};

// 统计间隔时间
const int STAT_TIME = 5;

// 统计信息
struct StatInfo
{
    int fd;
    int count;
};

#define UPPER_PRIM(Value) upper_prime(value, 0)

//禁用拷贝构造和赋值
#define DISALLOW_COPY_AND_ASSIGN(TypeName)          \
    TypeName(const TypeName&) = delete;             \
    TypeName& operator=(const TypeName&) = delete;

//针对数据 结构浅拷贝赋值
#define ASSIGEN(TypeName)                \
    TypeName& operator=(const TypeName& src){\
    memcpy(this, &src, sizeof(*this));\
    return *this;\
    }

//构造函数 Clear函数 清零
#define ZERO_CONSTRUCT_AND_CLEAR(TypeName)      \
    TypeName(){\
        memset(this, 0, sizeof(*this));\
    }\
    void clear(){\
        memset(this, 0, sizeof(*this));\
    }

#define BEGIN_STRCUT(StructName) struct StructName {\
    inline void reset();\
    inline void resume();\
    inline bool serialization(std::string& str);\
    inline bool deserialization(const char* str, size_t len);

#define END_STRCUT    };

#define ROUNDUP(n, width) (((n) + (width) - 1) & ~((width) - 1))

#define ROUNDDOWN(n, width) ((n) & ~(-width))

#define MATHMOD(n, width) (n)%(width)

// UP(A/B) = int((A+B-1)/B)
#define DIV_ROUND_UP(n, d) (((n) + (d) - 1) / (d))

#define UNUSE_ARG(arg) ((void)arg)

#define CHECK_FUNC_RET(func) do { int ret = func(); if (ret != 0) return ret;} while(0)

#define SAFE_DELETE(p) do { delete (p); (p) = NULL; } while(0)
#define SAFE_DELETE_ARRAY(p) do { delete [] (p); (p) = NULL; } while(0)
#define SAFE_FREE(p) do { free (p); (p) = NULL; } while(0)

#define LOGF(severity, format, ...)\
	 do{\
		char buff[512];int32_t size;\
        size = snprintf(buff, 512, format, ##__VA_ARGS__);\
        buff[size]='\0'; LOG(severity) << buff;\
     }while (0);

#define MARCO_ARGS_CONS(a, b) a##b

#define SYNTHESIZE(VarType, VarName) \
    private: VarType MARCO_ARGS_CONS(VarName, _); \
    public: inline VarType VarName(void) const { return MARCO_ARGS_CONS(VarName, _); } \
    public: inline void set_##VarName(VarType VarName) { MARCO_ARGS_CONS(VarName, _) = VarName; }

#define SYNTHESIZE_REF(VarType, VarName) \
    private: VarType MARCO_ARGS_CONS(VarName, _); \
    public: inline VarType* mutable_##VarName(void) { return &(MARCO_ARGS_CONS(VarName, _)); }\
    public: inline VarType& VarName(void) { return MARCO_ARGS_CONS(VarName, _); } \
    public: inline const VarType& const_##VarName(void) const {return  MARCO_ARGS_CONS(VarName, _);} \
    public: inline void set_##VarName(const VarType& VarName) { MARCO_ARGS_CONS(VarName, _) = VarName; }\
	public: template<class T>\
			inline inc_##VarName(T va){\
			static_assert(std::is_integral<T>::value && std::is_same<T,VarType>::value, "type error");\
			MARCO_ARGS_CONS(VarName, _) += va;\
		}

#define SYNTHESIZE_UNION2(VarType1, Vartype2, VarName)\
    union Union##VarName{Union##VarName(){} VarType1 filed1; Vartype2 filed2;} MARCO_ARGS_CONS(VarName, _);\
    public: inline VarType1* mutable_##VarName_##VarType1(){ return &MARCO_ARGS_CONS(VarName, _).filed1;}\
    public: inline VarType2* mutable_##VarName_##VarType2(){ return &MARCO_ARGS_CONS(VarName, _).filed2;}\
    public: inline const VarType1& get_##VarName_##VarType1(){return MARCO_ARGS_CONS(VarName, _).filed1;}\
    public: inline const VarType2& get_##VarName_##VarType2(){return MARCO_ARGS_CONS(VarName, _).filed2;}


#define SYNTHESIZE_UNION3(VarType1, Vartype2, Vartype3, VarName)\
    union Union##VarName{Union##VarName(){} VarType1 filed1; Vartype2 filed2; VarType3 filed3;} MARCO_ARGS_CONS(VarName, _);\
    public: inline VarType1* mutable_##VarName_##VarType1(){ return &MARCO_ARGS_CONS(VarName, _).filed1;}\
    public: inline VarType2* mutable_##VarName_##VarType2(){ return &MARCO_ARGS_CONS(VarName, _).filed2;}\
    public: inline VarType3* mutable_##VarName_##VarType3(){ return &MARCO_ARGS_CONS(VarName, _).filed3;}\
    public: inline const VarType1& get_##VarName_##VarType1(){return MARCO_ARGS_CONS(VarName, _).filed1;}\
    public: inline const VarType2& get_##VarName_##VarType2(){return MARCO_ARGS_CONS(VarName, _).filed2;}\
    public: inline const VarType3& get_##VarName_##VarType3(){ return MARCO_ARGS_CONS(VarName, _).filed3;}


#define SYNTHESIZE_UNION4(VarType1, Vartype2, Vartype3,Vartype4,VarName)\
    union Union##VarName{Union##VarName(){} VarType1 filed1; Vartype2 filed2;VarType3 filed3; Vartype4 filed4;} MARCO_ARGS_CONS(VarName, _);\
    public: inline VarType1* mutable_##VarName_##VarType1(){ return &MARCO_ARGS_CONS(VarName, _).filed1;}\
    public: inline VarType2* mutable_##VarName_##VarType2(){ return &MARCO_ARGS_CONS(VarName, _).filed2;}\
    public: inline VarType3* mutable_##VarName_##VarType3(){ return &MARCO_ARGS_CONS(VarName, _).filed3;}\
    public: inline VarType4* mutable_##VarName_##VarType4(){ return &MARCO_ARGS_CONS(VarName, _).filed4;}\
    public: inline const VarType1& get_##VarName_##VarType1(){return MARCO_ARGS_CONS(VarName, _).filed1;}\
    public: inline const VarType2& get_##VarName_##VarType2(){return MARCO_ARGS_CONS(VarName, _).filed2;}\
    public: inline const VarType3& get_##VarName_##VarType3(){return MARCO_ARGS_CONS(VarName, _).filed3;}\
    public: inline const VarType4& get_##VarName_##VarType4(){return MARCO_ARGS_CONS(VarName, _).filed4;}

#define SYNTHESIZE_PTR(VarType, VarName) \
    private: VarType* MARCO_ARGS_CONS(VarName, _); \
    public: inline VarType* mutable_##VarName(void){\
            if ((MARCO_ARGS_CONS(VarName, _)) == NULL){\
				 MARCO_ARGS_CONS(VarName, _) = new VarType();\
                 DLOG(INFO) <<"syn_ptr new  ptr  "#VarName<<"  " << (void*)MARCO_ARGS_CONS(VarName, _);
			} \
			return (MARCO_ARGS_CONS(VarName, _)); \
        } \
    public: inline VarType* VarName(void) const { return MARCO_ARGS_CONS(VarName, _); } \
    public: inline void clear_##VarName(){\
            if ((MARCO_ARGS_CONS(VarName, _)) != NULL) {\
				 DLOG(INFO) <<"syn_ptr del  ptr  "#VarName << "  "<<(void*)MARCO_ARGS_CONS(VarName, _);\
				delete MARCO_ARGS_CONS(VarName, _);\
				MARCO_ARGS_CONS(VarName, _) = NULL;
			}\
        }\
    public: inline void null_##VarName(){ MARCO_ARGS_CONS(VarName, _) = NULL;}

#define SYNTHESIZE_CHAR_ARRAY(VarName, VarLength) \
    private: char MARCO_ARGS_CONS(VarName, _)[VarLength]; \
    public: inline const char* VarName(void) const { return MARCO_ARGS_CONS(VarName, _); } \
    public: inline void set_##VarName(const char* VarName) { \
            assert(strlen(VarName) < VarLength);\
            if(unlikely(strlen(VarName) >= VarLength))return;\
            strncpy(MARCO_ARGS_CONS(VarName, _), VarName, strlen(VarName)+1);\
		}\
    public: inline void clear_##VarName() { MARCO_ARGS_CONS(VarName, _)[0] = '\0';}

#define SYNTHESIZE_ARRAY(VarType, VarName, VarLength) \
    private: VarType MARCO_ARGS_CONS(VarName, _)[VarLength]; \
    private: uint64_t MARCO_ARGS_CONS(VarName, _bit)[DIV_ROUND_UP(VarLength, 64)];\
    public: inline VarType* mutable_##VarName(int32_t index) { \
            assert(index >= 0 && index < VarLength);\
            if(unlikely (index < 0 || index >= VarLength)) return NULL; \
            return &(MARCO_ARGS_CONS(VarName, _)[index]); \
        } \
        public: inline const VarType& VarName(int32_t index)const { \
            assert(index >= 0 && index < VarLength);\
            if(unlikely (index < 0 || index >= VarLength)) return MARCO_ARGS_CONS(VarName, _)[0]; \
            return MARCO_ARGS_CONS(VarName, _)[index]; \
        } \
    public: inline void set_##VarName(int32_t index, const VarType& VarName) { \
            assert(index >= 0 && index < VarLength);\
            if(unlikely (index < 0 || index >= VarLength)) return;\
            MARCO_ARGS_CONS(VarName, _)[index] = VarName; \
        } \
      public: template <class Predicate>\
        int32_t find_##VarName(const Predicate& pred){\
            for(int32_t i = 0; i < VarLength; i++){\
                if (pred(MARCO_ARGS_CONS(VarName, _)[i])) return i;\
            }\
            return -1;\
        }\
    public: inline void clear_##VarName(){\
            memset(&MARCO_ARGS_CONS(VarName, _), 0, sizeof(MARCO_ARGS_CONS(VarName, _))); \
            memset(&MARCO_ARGS_CONS(VarName, _bit), 0 , sizeof(uint64_t)*DIV_ROUND_UP(VarLength, 64));\
        } \
    public: inline void copy_##VarName(const VarType* srcVar, size_t len){\
            assert(len <= VarLength);\
            if(unlikely (len > VarLength)) return;\
            memcpy(&MARCO_ARGS_CONS(VarName, _), srcVar, sizeof(VarType)*len);\
        }\
   public: inline int32_t VarName##_size() const { return VarLength; }\
   public: inline bool has_##VarName##_index(int32_t index){\
			assert(index < VarLength);\
			if(index>= VarLength) return false;\
			return MARCO_ARGS_CONS(VarName, _bit)[index/64]|&(1<<(index%64));\
		}\
    public: inline void clear_has_##VarName##_index(int32_t index){\
			assert(index < VarLength);\
			if(index >= VarLength) return;\
			MARCO_ARGS_CONS(VarName, _bit)[index/64] &= ~(1<<(index%64));\
		}\
    public: inline void set_has_##VarName##_index(int32_t index){\
			assert(index < VarLength);\
			if(index >= VarLength) return;\
			MARCO_ARGS_CONS(VarName, _bit)[index/64] |= (1<<(index%64));\
		}\
    public :inline void clear_##VarName(int32_t index){\
           assert(index < VarLength);\
           if(index >= VarLength) return;\
           MARCO_ARGS_CONS(VarName, _bit)[index/64] &= ~(1<<(index%64));\
		}
   
#define SYNTHESIZE_FIXED_VECTOR(VarType, VarName, VarLength) \
    private: int16_t  MARCO_ARGS_CONS(VarName,size);\
    private: VarType MARCO_ARGS_CONS(VarName, _)[VarLength]; \
    public: inline VarType* mutable_##VarName(int32_t index){ \
            assert(index >= 0 && index < MARCO_ARGS_CONS(VarName,size) );\
            if(unlikely (index < 0 || index >= MARCO_ARGS_CONS(VarName,size))) return NULL; \
            return &(MARCO_ARGS_CONS(VarName, _)[index]); \
        } \
    public: inline const VarType& VarName(int32_t index) const{ \
            assert(index >= 0 && index < MARCO_ARGS_CONS(VarName,size) );\
            if(unlikely (index < 0 || index >= MARCO_ARGS_CONS(VarName,size) )) \
                 return MARCO_ARGS_CONS(VarName, _)[0]; \
            return MARCO_ARGS_CONS(VarName, _)[index]; \
        } \
    public: inline void set_##VarName(int32_t index, const VarType& VarName){\ 
assert(index >= 0 && index < MARCO_ARGS_CONS(VarName, size)); \
	if (unlikely(index < 0 || index >= MARCO_ARGS_CONS(VarName, size))) return; \
		MARCO_ARGS_CONS(VarName, _)[index] = VarName; \
		}\
	public: inline int32_t add_##VarName(const VarType& VarName) {
		\
			assert(MARCO_ARGS_CONS(VarName, size) < VarLength); \
			if (unlikely(MARCO_ARGS_CONS(VarName, size) >= VarLength)) return -1; \
				MARCO_ARGS_CONS(VarName, _)[MARCO_ARGS_CONS(VarName, size)] = VarName; \
				return MARCO_ARGS_CONS(VarName, size)++; \
		}\
	public: inline VarType* add_##VarName() {
				\
			assert(MARCO_ARGS_CONS(VarName, size) < VarLength); \
			if (unlikely(MARCO_ARGS_CONS(VarName, size) >= VarLength)) return NULL; \
			return &MARCO_ARGS_CONS(VarName, _)[MARCO_ARGS_CONS(VarName, size)++]; \
		}\
	 public: template <class T>\
			int32_t find_integer_##VarName(const T& va){\
			static_assert(std::is_integral<T>::value && std::is_same<T, VarType>::value, "type error"); \
			for (int32_t i = 0; i < MARCO_ARGS_CONS(VarName, size); i++) {\
				if (MARCO_ARGS_CONS(VarName, _)[i] == va) return i; \
			}\
			return -1; \
		}\
	public: template <class Predicate>\
			int32_t find_##VarName(const Predicate& pred) {\
            for(int32_t i = 0; i < MARCO_ARGS_CONS(VarName,size); i++){\
                if (pred(MARCO_ARGS_CONS(VarName, _)[i])) return i;\
            }\
            return -1;\
        }\
    public: template <class Behavior>\
            int32_t feach_##VarName(const Behavior& beh){\
            int32_t counter = 0;\
            for(int32_t i = 0; i < MARCO_ARGS_CONS(VarName,size); i++){\
                if (beh(MARCO_ARGS_CONS(VarName, _)[i])) counter++;\
            }\
            return counter;\
		}\
	  public: template <class Predicate>\
            int32_t delete_##VarName(const Predicate& pred){\
            int32_t counter = 0;\
            for(int32_t i = MARCO_ARGS_CONS(VarName,size) - 1;i >= 0; i--){\
                if (pred(MARCO_ARGS_CONS(VarName, _)[i])){\
					MARCO_ARGS_CONS(VarName, _)[i] = MARCO_ARGS_CONS(VarName, _)[--MARCO_ARGS_CONS(VarName,size)];\
                    counter++;\
				}\
            }\
            return counter;\
        }\
    public: inline void delete_##VarName(int32_t index){\
            assert(index >= 0 && index < MARCO_ARGS_CONS(VarName,size));\
            if(unlikely (index < 0 || index >= MARCO_ARGS_CONS(VarName,size))) \
                 return; \
            MARCO_ARGS_CONS(VarName, _)[index] = MARCO_ARGS_CONS(VarName, _)[--MARCO_ARGS_CONS(VarName,size)];\
        }\
    public: inline void copy_##VarName(const VarType* srcVar, size_t len){\
            assert(len <= VarLength);\
            if(unlikely(len > VarLength)) return;\
            memcpy(&MARCO_ARGS_CONS(VarName, _), srcVar, sizeof(VarType)*len);\
            MARCO_ARGS_CONS(VarName,size) = len;\
        }\
    public: inline void clear_##VarName(){ \
            MARCO_ARGS_CONS(VarName,size) = 0;\
        } \
    public: inline int16_t VarName##_size() const  { return MARCO_ARGS_CONS(VarName,size); }\
    public: inline int32_t VarName##_capitiy() const { return VarLength; }

//
#define hash_map(T) std::unordered_map<T, int32_t, _synth_helpper::hash<T>>

#define SYNTHESIZE_FIXED_MAP(KeyType, VarType, VarName, VarLength)\
	private: hash_map(KeyType)* VarName##index_map_ = NULL;\
	private: char VarName##flag_[VarLength];\
	private: VarType MARCO_ARGS_CONS(VarName, _)[VarLength]; \
	public: VarType* add_##VarName(KeyType key){\
			if(VarName##index_map_ == NULL) VarName##index_map_ = new hash_map(KeyType)();\
			if( VarName##index_map_->find(key) !=  VarName##index_map_->end()) return NULL;\
			for (int32_t i = 0; i < VarLength; i++){\
				if(VarName##flag_[VarLength] == 0){\
					VarName##flag_[VarLength] = 1;\
					return &MARCO_ARGS_CONS(VarName, _)[i];\
				}\
			}\
			return NULL;\
		}\
	public: int32_t add_##VarName(const VarType& value) {\
			if(VarName##index_map_ == NULL) VarName##index_map_ = new hash_map(KeyType)();\
			KeyType key == _synth_helpper::get_key(value);\
			if(VarName##index_map_->find(key) !=  VarName##index_map_->end()) return -1;\
			for (int32_t i = 0; i < VarLength; i++){\
				if(VarName##flag_[VarLength] == 0){\
					VarName##flag_[VarLength] = 1;\
					MARCO_ARGS_CONS(VarName, _)[i] = value;\
					return i;\
				}\
			}\
			return -1; \
		}\
	public: int32_t delete_##VarName(uint64_t key){\
			if (VarName##index_map_ == NULL) return -1; \
			auto itr = VarName##index_map_->find(key);\
			if(itr ==  VarName##index_map_->end()) return -1;\
			int32_t index = itr.second;\
			if(unlikely(index < 0 || index >= VarLength) return -1;\
			VarName##flag_[index] = 0;\
			VarName##index_map_.erase(itr);\
			return 0;\
		}\
	public: VarType* find_##VarName(const KeyType& key){\
			if(VarName##index_map_ == NULL) return NULL;\
			auto itr = VarName##index_map_->find(key);\
			if(itr ==  VarName##index_map_->end()) return -1;\
			int32_t index = itr.second;\
			assert(index>=0 &&index <VarLength && VarName##flag_[index] == 1);\
			return &MARCO_ARGS_CONS(VarName, _)[i];\
		}\
	template<class Predicate>\
	public: VarType * find_##VarName(const Predicate& pred) {\
			for (int32_t i = 0; i < VarLength; i++){\
				if(VarName##flag_[VarLength] == 1){\
					if(pred(MARCO_ARGS_CONS(VarName, _)[i]))\
						return &MARCO_ARGS_CONS(VarName, _)[i];\
				}\
			}\
			return NULL;\
		}\
	public: void reload_##VarName(){\
		VarName##index_map_ = new hash_map(KeyType)();\
		for(int32_t i = 0; i < VarLength; i++){\
			if(VarName##flag_[i] == 1){\
				KeyType key == _synth_helpper::get_key(value);\
				VarName##index_map_[key] = i;\
				if (std::is_standard_layout<VarType>::value) continue;\
				_synth_helpper::reload(MARCO_ARGS_CONS(VarName, _)[i]);
			}\
		}\
		if (std::is_integral<uint64_t>::value == false) reload();\ //for check struct reload member function only
	}\
	public:void clear_##VarName() {\
		SAFE_DELETE(VarName##index_map_); \
		memset(VarName##flag_, 0, sizeof(VarName##flag_));\
	}\
	public:inline int32_t VarName##_size(){\
		if(VarName##index_map_== NULL) return 0;\
		return VarName##index_map_->size();\
	}\
	public:inline int32_t VarName##_capitiy(){\
		return VarLength;\
	}\
	public:inline int32_t free_space_##VarName(){\
		return VarName##_capitiy()- VarName##_size();\
	}



	

namespace _synth_helpper {

template<class T>
inline uint64_t get_key(const T& va) {
	return value.key();
}

template<>
inline uint64_t get_key(const uint64_t& va) {
	return va;
}

template<>
inline uint32_t get_key(const uint32_t& va) {
	return (uint64_t)va;
}

template<>
inline int64_t get_key(const int64_t& va) {
	return (uint64_t)va;
}
template<>
inline int32_t get_key(const int32_t& va) {
	return (uint64_t)va;
}

template<class T>
inline void reload(T& va) {
	va.reload()
}
template<>
inline void reload(uint64_t& va) {
}

template<>
inline void reload(int64_t& va) {
}

template<>
inline void reload(uint32_t& va) {
}

template<>
inline void reload(int32_t& va) {
}

struct hash<T>{	
	size_t operator()(const T _Keyval) const{
		std::hash<T> k(_Keyval);
		return k();
	}
};

struct hash<> {
	size_t operator()(const uint64_t _Keyval) const {
		return _Keyval;
	}
};
struct hash<> {
	size_t operator()(const int64_t _Keyval) const {
		return _Keyval;	
	}
};

struct hash<> {
	size_t operator()(const uint32_t _Keyval) const {
		return (uint64_t)_Keyval * 2654435761 % 2 ^ 32;
	}
};


struct hash<> {
	size_t operator()(const int32_t _Keyval) const {
		return (uint64_t)_Keyval * 2654435761 % 2 ^ 32;
	}
};


}

		
#endif // _COMM_DEF_H_

