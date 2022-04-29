#include "bot.h"
namespace BOT
{
lint pieces=startpieces,board=startboard;
int col;
int x0,y0,x1,y1;
lint pres,bres;
void move(int x0,int y0,int x1,int y1)
{
	pieces|=(1ll<<(x1*7+y1))|maskc[x1*7+y1];
	board|=(1ll<<(x1*7+y1));
	if(!((1ll<<(x1*7+y1))&maskc[x0*7+y0]))
		board&=~(1ll<<(x0*7+y0));
	pieces=(~pieces)&board;
}

int popcount(lint a)
{
	int ret=0;
	while(a) ret++,a&=a-1;
	return ret;
}
int log2(lint a)
{
	int ret=0;
	for(int k=32;k;k>>=1)
		if(a>>k) ret+=k,a>>=k;
	return ret;
}

int dfs(lint pn,lint bn,int depth,int alp,int bet)
{
	if(!depth) return popcount(pn)-popcount((~pn)&bn);
	int ret=-50;
	vector<lint> pv,bv;
	lint tmp=0;
	for(lint s=pn;s;s&=s-1) tmp|=maskc[log2(s&-s)];
	tmp&=~bn;while(tmp)
	{
		pv.emplace_back(pn|(tmp&-tmp)|maskc[log2(tmp&-tmp)]),
		bv.emplace_back(bn|(tmp&-tmp)),
		tmp&=tmp-1;
	}
	for(lint s=pn;s;s&=s-1)
	{
		tmp=maskj[log2(s&-s)];tmp&=~bn;while(tmp)
		{
			pv.emplace_back(pn|(tmp&-tmp)|maskc[log2(tmp&-tmp)]),
			bv.emplace_back((bn|(tmp&-tmp))&~(s&-s));
			tmp&=tmp-1;
		}
	}
	if(pv.empty()) pv.emplace_back(pn),bv.emplace_back(bn);
	int itmp=0;
	for(int i=0;i<pv.size();i++)
	{
		itmp=-dfs((~pv[i])&bv[i],bv[i],depth-1,-bet,-alp);
		if(itmp>ret)
		{
			ret=itmp;
			alp=max(alp,itmp);
			if(depth==ddepth) pres=pv[i],bres=bv[i];
		}
		if(alp>=bet) break;
	}
	return alp;
}
pair<pair<int,int>,pair<int,int> > main(long long iboard,long long ipieces,bool nowplayer)
{
	board=iboard,pieces=ipieces;
	if(nowplayer) pieces=board^pieces;
	dfs(pieces,board,ddepth,-50,50);
	
	int tmp=0;
	lint ts=0;
	ts=bres^(bres&board);
	while(ts!=(1ll<<tmp)) tmp++;
	x1=tmp/7,y1=tmp%7;
	ts=board^(bres&board);
	if(!ts)
	{
		ts=pieces&maskc[tmp];
		ts=ts&-ts;
	}
	tmp=0;
	while(ts!=(1ll<<tmp)) tmp++;
	x0=tmp/7,y0=tmp%7;
	
	
	return make_pair(make_pair(x0,y0),make_pair(x1,y1));
}

}