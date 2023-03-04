#include "../../include/myPrintk.h"
#include "../../include/mystring.h"

//dPartition 是整个动态分区内存的数据结构
typedef struct dPartition{
	unsigned long size;
	unsigned long firstFreeStart; 
} dPartition;	//共占8个字节

#define dPartition_size ((unsigned long)0x8)

void showdPartition(dPartition *dp){
	myPrintk(0x5,"dPartition(start=0x%x, size=0x%x, firstFreeStart=0x%x)\n", dp, dp->size,dp->firstFreeStart);
}


// EMB每一个block的数据结构，userdata可以暂时不用管。
typedef struct EMB{
	unsigned long size;
	union {
		unsigned long nextStart;    // if free: pointer to next block
        unsigned long userData;		// if allocated, blongs to user
	};	                           
} EMB;	//共占8个字节

#define EMB_size ((unsigned long)0x8)

void showEMB(struct EMB * emb){
	myPrintk(0x3,"EMB(start=0x%x, size=0x%x, nextStart=0x%x)\n", emb, emb->size, emb->nextStart);
}

//=================接口规定=====================
//分配完成之后，返回的首地址为可以直接使用的首地址，即原emb结构union的首地址
//进行释放时，传入的是可以用的首地址，所以要后退4bytes才能得到该块的大小

unsigned long dPartitionInit(unsigned long start, unsigned long totalSize){
	//本函数需要实现！！！
	/*功能：初始化内存。
	1.在地址start处，首先是要有dPartition结构体表示整个数据结构(也即句柄)。
	2.然后，一整块的EMB被分配（以后使用内存会逐渐拆分），在内存中紧紧跟在dP后面，然后dP的firstFreeStart指向EMB。
	3.返回start首地址(也即句柄)。

	注意有两个地方的大小问题：
		第一个是由于内存肯定要有一个EMB和一个dPartition，totalSize肯定要比这两个加起来大。
		第二个注意EMB的size属性不是totalsize，因为dPartition和EMB自身都需要要占空间。
		!!!但是union在分配的时候可以用，所以是减去EMB_size/2
	*/
	if(totalSize < dPartition_size + EMB_size)
		return 0;

	dPartition partition;
	partition.size = totalSize - dPartition_size;
	partition.firstFreeStart = start + dPartition_size;
	memcpy((void *)start, &partition, dPartition_size);

	EMB emb;
	emb.size = totalSize - dPartition_size - EMB_size/2;
	emb.nextStart = 0; //起始只有一块emb
	memcpy((void *)(start + dPartition_size), &emb, EMB_size);

	return start;
}

void dPartitionWalkByAddr(dPartition* dp){
	//unsigned long dp
	//本函数需要实现！！！
	/*功能：本函数遍历输出EMB 方便调试
	1.先打印dP的信息，可调用上面的showdPartition。
	2.然后按地址的大小遍历EMB，对于每一个EMB，可以调用上面的showEMB输出其信息
	*/
	//首先打印dp信息，这里的dp传递的是指针
	showdPartition(dp);
	//按地址大小遍历EMB
	//要求内存分配与回收要按地址高低排序
	EMB* pemb = (EMB*) dp->firstFreeStart;
	do{
		showEMB(pemb);
		pemb = (EMB*) pemb->nextStart;
	}while( pemb != NULL);

}

//=================firstfit, order: address, low-->high=====================

/*注意在测量块的size时是要把union部分加上的！！！*/

unsigned long dPartitionAllocFirstFit(dPartition* dp, unsigned long size){
	//本函数需要实现！！！
	/*功能：分配一个空间
	1.使用firstfit的算法分配空间，当然也可以使用其他fit，不限制。
	2.成功分配返回首地址，不成功返回0
	3.从空闲内存块组成的链表中拿出一块供我们来分配空间(如果提供给分配空间的内存块空间大于size，我们还将把剩余部分放回链表中)，并维护相应的空闲链表以及句柄
	注意的地方：
		1.EMB类型的数据的存在本身就占用了一定的空间。

	*/
	//从链表第一项开始判断
	EMB* tmp;
	EMB* pemb = (EMB*) dp->firstFreeStart;
	while(pemb != NULL){
		if( pemb->size > size){
			//满足进行分配，并且修改链表
			//若有大于emb块的剩余空间进行切分
			if(pemb->size - size > EMB_size){
				EMB emb;
				unsigned long embstart = (unsigned long)pemb + EMB_size/2 + size;
				emb.size = pemb->size - size - EMB_size/2;
				//与上一个结点连接
				if(pemb == (EMB*) dp->firstFreeStart )
					dp->firstFreeStart = embstart;
				else{
					tmp = (EMB*) dp->firstFreeStart;
					while(tmp->nextStart != (unsigned long)pemb)
						tmp = (EMB*) tmp->nextStart;
					tmp->nextStart = embstart;
				}
				//与下一个结点连接
				emb.nextStart = pemb->nextStart; 
				pemb->size = size; 
				//将建好的emb放入内存中
				memcpy((void *)embstart, &emb, EMB_size);	
			}
			else{
				//剩余碎片太小，不再切割，转换为内部碎片
				//则不再创建新的emb，直接对链表进行重新连接
				if(pemb == (EMB*) dp->firstFreeStart )
					dp->firstFreeStart = pemb->nextStart;
				else{
					tmp = (EMB*) dp->firstFreeStart;
					while(tmp->nextStart != (unsigned long)pemb)
						tmp = (EMB*) tmp->nextStart;
					tmp->nextStart = pemb->nextStart;
				}
				pemb->size += EMB_size/2;
			}	
			return (unsigned long) ((unsigned long)pemb+ EMB_size/2);//返回地址为emb结构体size结束，union的首地址
		}
		else{
			pemb = (EMB*) pemb->nextStart;
		}
	}
	myPrintf(0x7, "no space for malloc!\n");
	return (unsigned long)0x0;
}

unsigned long dPartitionFreeFirstFit(dPartition* dp, unsigned long start){
	//本函数需要实现！！！
	/*功能：释放一个空间
	1.按照对应的fit的算法释放空间
	2.注意检查要释放的start~end这个范围是否在dp有效分配范围内
		返回1 没问题
		返回0 error
	3.需要考虑两个空闲且相邻的内存块的合并
	*/
	int mergeBefore = 0, mergeAfter = 0;
	unsigned long embstart = start - EMB_size/2;
	unsigned long embsize = ((EMB*)embstart)->size;

	//检查要释放的start~end这个范围是否在dp有效分配范围内
	if(embstart < (unsigned long)dp + dPartition_size || 
		start + embsize > (unsigned long)dp + dPartition_size + dp->size)
		return 0;
	else{
		//释放内存
		//寻找应该插入的位置
		EMB* p =  (EMB*) dp->firstFreeStart;
		EMB* pre = p;

		while( p != NULL && embstart > (unsigned long)p ){
			pre = p;
			p = (EMB*)p->nextStart;
		}
		//再次检查要释放的start~end这个范围是否在dp有效分配范围内
		//pre != p 和 p != NULL保证该emb不在头尾
		if(( pre != p && (unsigned long)pre + EMB_size/2 + pre->size > embstart) ||
			( p != NULL && start + embsize > (unsigned long)p) )
			return 0;
		//判断是否可以向前和向后合并
		if(start + embsize == (unsigned long)p)
			mergeAfter = 1;
		if((unsigned long)pre + EMB_size/2 + pre->size == embstart)
			mergeBefore = 1;
		//四种情况
		if(!mergeBefore && !mergeAfter){
			if(embstart > dp->firstFreeStart)
				pre->nextStart = embstart;
			else
				dp->firstFreeStart = embstart;

			((EMB*)embstart)->size = embsize;
			((EMB*)embstart)->nextStart = (unsigned long)p;
		}
		else if(!mergeBefore && mergeAfter){
			if(embstart > dp->firstFreeStart)
				pre->nextStart = embstart;
			else
				dp->firstFreeStart = embstart;

			((EMB*)embstart)->size = embsize + EMB_size/2 + p->size;
			((EMB*)embstart)->nextStart = p->nextStart;
		}	
		else if(mergeBefore && !mergeAfter){
			pre->nextStart = (unsigned long)p;
			pre->size += (embsize + EMB_size/2);
		}
		else{
			pre->size += ( EMB_size + embsize + p->size);
			pre->nextStart = p->nextStart;
		}		
		return 1;
	}
}

//wrap: we select firstFit, you can select another one
//user need not know this
unsigned long dPartitionAlloc(unsigned long dp, unsigned long size){
	return dPartitionAllocFirstFit((dPartition *)dp,size);
}

unsigned long dPartitionFree(unsigned long	 dp, unsigned long start){
	return dPartitionFreeFirstFit((dPartition *)dp,start);
}
