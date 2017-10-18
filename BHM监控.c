1. 
#define LISTEN_MAXNUM 64
typedef struct BHMLISTENINFO
{
	int count;
	int	status[LISTEN_MAXNUM];
	int	id[LISTEN_MAXNUM];
	int	numBHM[LISTEN_MAXNUM];
}BHMLisInfo,*pBHMLisInfo;
2.PaService.c 
void PaService::init(void){
	knBzero(&cBHMLisInfo,sizeof(BHMLisInfo));
	this->m_pBHMMutex = knCreateMutex();
}
void PaService::close(void)
{
	if(NULL != this->m_pBHMMutex)
	{
		knDeleteMutex(this->m_pBHMMutex);
	}
}
void PaService::setBHMLisInfo(BHMLisInfo cBHMLisInfo)
{
	this->cBHMLisInfo = cBHMLisInfo;
}

BHMLisInfo PaService::getBHMLisInfo(void)
{
	return this->cBHMLisInfo;
}

void PaService::ClearBHMLisStatusInfo()
{
	knBzero(cBHMLisInfo.status,LISTEN_MAXNUM);
}

void PaService::BHMMutexLock(void)
{
	knMutexLock(this->m_pBHMMutex);
}

void PaService::BHMMutexUnlock(void)
{
	knMutexUnlock(this->m_pBHMMutex);
}

static void myCb(void *icsd_)
{
	PaService *pas = NULL;
	BHMLisInfo	cBHMLisInfo = {0};
	pas = dvGetTemp(icsd_);
	cBHMLisInfo = pas->getBHMLisInfo();
	cBHMLisInfo.count = c_size;
	pas->setBHMLisInfo(cBHMLisInfo);
}
static void BHMListenCb(void *icsd_)
{
	PaService *pas = NULL;
	tS1Service *ss = NULL;
	BHMLisInfo	cBHMLisInfo = {0};
	int i;
	void *dblt = NULL;
	void *dbht = NULL;
	pas = (PaService*)dvGetTemp(icsd_);
	dblt = dvGetDbMgt1(icsd_);
	dbht = dbGetTheDbServiceEnginet(dblt,0);
	ss = (tS1Service*)dvGetServSrc(icsd_);
	
	for(::)
	{
		pas->WaitMyEventp();
		pas->BHMMutexLock();
		cBHMLisInfo = pas->getBHMLisInfo();
		for(i = 1;i < cBHMLisInfo.count;i++)
		{
			if(cBHMLisInfo.status[i] == 0)
			{
				cBHMLisInfo.numBHM[i]++;
				if(cHHMLisInfo.numBHM[i] > 3)
				{
					SendBHMtoDB(dbht,i);
					cBHMLisInfo.numBHM[i] = 0;
				}
				else
				{
					setBHM(icsd_,i,cBHMLisInfo.id[i]);
					cBHMLisInfo.id[i]++;
					if(cBHMLisInfo.id[i] > 9)
					{
						cBHMLisInfo.id[i] = 0;
					}
				}
			}
			else
			{
				cHHMLisInfo.numBHM[i] = 0;
			}
		}
		pas->setBHMLisInfo(cBHMLisInfo);
		pas->ClearBHMLisStatusInfo();
		pas->BHMMutexUnlock();
	}
}
3.AnalyToHandle.c
int saveRecvDataToDB(char *ip,Cache *cache,int c_size,char *tableName,char *tableID,void *icsd_,void *dbh_)
{
	BHMLisInfo	cBHMLisInfo = {0};
	pS->BHMMutexLock();
	cBHMLisInfo = pS->getBHMLisInfo();
	cBHMLisInfo.status[TCPID] = 1;
	pS->setBHMLisInfo(cBHMLisInfo);
	pS->BHMMutexUnlock();
}
4.PaService.h
void setBHMLisInfo(BHMLisInfo cBHMLisInfo);

BHMLisInfo getBHMLisInfo(void);

void  ClearBHMLisStatusInfo();

BHMLisInfo cBHMLisInfo;

void  BHMMutexLock();

void  BHMMutexUnlock();

void *m_pBHMMutex;

5.ReslvToPack.c
PaService *pS = NULL;
BHMLisInfo	cBHMLisInfo = {0};
pS = (PaService*)dvGetTemp(icsd_);
pS->BHMMutexLock();
cBHMLisInfo = pS->getBHMLisInfo();
cBHMLisInfo.count = ccount;
pS->setBHMLisInfo(cBHMLisInfo);
pS->BHMMutexUnlock();








