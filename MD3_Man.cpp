#include "MD3_Man.h"
#include "Md3.h"
#include <gl\glu.h>									// Header File For The GLu32 Library


//#define MODEL_PATH  "\Models\\Eagle.pk3"
//#define MODEL_NAME	"Eagle_hi"
CModelMD3
	//Architectonic
	Alcove_md3[1], Eagle_md3[1], Eagle_md3_[1],//Eagle_md3[0][x] standing, Eagle_md3[1][x] fallen
	Column_md3[1], 
	//Furniture
	Chair_md3[1], WoodenTable_md3[1], Table_md3[1], Bed_md3[1],
	//Lights
	Lamp_md3[1], Chandelier_md3[1], ChandelierGlass_md3[1], Torcher_md3[1],
	//Knights
	Sword_md3[1], Spears_md3[1], Holder_md3[1],
	//Plants
	Roots_md3[1], RoundTree_md3[1], VasedTree_md3[1],
	//Remains
	LyingSkeleton_md3[1], Well_md3[1], CellDoor_md3[1], CelledSkeleton_md3[1],
	//Treasures
	Key_md3[1], Vase_md3[1], Crosses_md3[8], Goblets_md3[8], Crowns_md3[8], TreasureBox_md3[1];
int IronEagles;
int MapMD3s[64][64];
short int MapMods[64][64];//Map Modificators
short int MapSMods[64][64];//Map Skin Modificators
//int MapMods[64][64];//Map Modificators

////////////////////////////////////////////OPTMIZE
//1 Eron-Eagle
//-1 Eron-Eagle fallen
//2 Potrait
//3 Poster
//4 Treasure
short int Crosses, Goblets;//Number of kinds

//int temporary_all_purpose_counter;

/*
float g_RotateX		  =    0.0f;		// This is the current value at which the model is rotated
float g_RotationSpeed =    0.1f;		// This is the speed that our model rotates. 
float g_TranslationX = 0.f;
float g_TranslationY = 0.6f;
//float g_TranslationY = 150.0f;
float g_TranslationZ  = -500.0f;		// This stores our distance away from the model
//float g_TranslationZ  = -500.0f;		// This stores our distance away from the model
*/

extern "C" void ResetMapModels(void){
	int i,j;
	srand(244);//Intialization of  Random Number Generator
	for(i=0;i<64;i++)for(j=0;j<64;j++){
Prt:MapMods[i][j]=floor(rand()*(8+1)/RAND_MAX);//fill by random 0-7
	if (MapMods[i][j]>=8) goto Prt;}
	for(i=0;i<64;i++)for(j=0;j<64;j++){
Prs:MapSMods[i][j]=floor(rand()*(8+1)/RAND_MAX);//fill by random 0-7
	if (MapSMods[i][j]>=8) goto Prs;}
}


extern "C" void AddIronEagle(int *x, int * y, int *state){//State for load only
	MapMD3s[*x][*y]=1;//;+-2**state;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////
//Architectonic
///////////////////////////////////////////////////////////////////////////////////////////////////////////
extern "C" void DrawIronEagle(float* x, float* y, int* x_, int* y_, float* z,
								float* d/*direction*/,int* lod){
	Alcove_md3[0].X=*x; 
	Alcove_md3[0].Y=-*y;
	Alcove_md3[0].Z=*z;
	Alcove_md3[0].D=*d;
	Alcove_md3[0].DrawModel(*lod);
	if (MapMD3s[*x_][*y_]==1){
		Eagle_md3[0].X=*x; 
		Eagle_md3[0].Y=-*y;
		Eagle_md3[0].Z=*z;//z;
		Eagle_md3[0].D=*d;
		Eagle_md3[0].DrawModel(*lod, 0);//standing
		}else{
		Eagle_md3_[0].X=*x; 
		Eagle_md3_[0].Y=-*y;
		Eagle_md3_[0].Z=*z;//z;
		Eagle_md3_[0].D=*d;
		Eagle_md3_[0].DrawModel(*lod, 0);//falling
}		}

extern "C" void FloorIronEagle(int* x, int* y){//Fix!!!!!!!!!!!!
	/*
	//int i;
	t3DModel *pModel;
	// This line gives us a pointer to the model that we want to change
	pModel = Eagle_md3_[2].GetModel(0);

	// To cycle through the animations, we just increase the model's current animation
	// by 1.  You'll notice that we also mod this result by the total number of
	// animations in our model, to make sure we go back to the beginning once we reach
	// the end of our animation list.  
/*
	// Increase the current animation and mod it by the max animations
	//pModel->currentAnim = (pModel->currentAnim + 1) % (pModel->numOfAnimations);

	// Set the current frame to be the starting frame of the new animation
	//pModel->nextFrame = pModel->pAnimations[pModel->currentAnim].startFrame;*/
//	for (i=0; i<IronEagles; i++);
//		if ((IronEaglesX[i]==*x)&&(IronEaglesY[i]==*y)/*&&(!IronEaglesState[i])*/);//{
	/*if (MapMD3s[*x][*y]==1){
		pModel->currentAnim=1;	
		pModel->nextFrame = pModel->pAnimations[pModel->currentAnim].startFrame;
		MapMD3s[*x][*y]=-1;
		}
			//break;
			//}
	*/}


extern "C" void DrawColumn(float* x, float* y,int* lod){
	Column_md3[0].X=*x; 
	Column_md3[0].Y=*y;
	Column_md3[0].Z=-0.6;//*z;
	Column_md3[0].D=0;//*d;
	Column_md3[0].DrawModel(*lod,0);
}

extern "C" void DrawMarbleColumn(float* x, float* y,int* lod){
	Column_md3[0].X=*x; 
	Column_md3[0].Y=*y;
	Column_md3[0].Z=-0.6;//*z;
	Column_md3[0].D=0;//*d;
	Column_md3[0].DrawModel(*lod,1);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
//Furniture
///////////////////////////////////////////////////////////////////////////////////////////////////////////
extern "C" void DrawTableChairs(float* x, float* y,int* lod){
	Chair_md3[0].X=*x; 
	Chair_md3[0].Y=*y+0.3;
	Chair_md3[0].Z=-0.6;//*z;
	Chair_md3[0].D=0;//*d;
	Chair_md3[0].DrawModel(*lod);
	Chair_md3[0].X=*x+0.3; 
	Chair_md3[0].Y=*y;
	Chair_md3[0].D=2;//*d;
	Chair_md3[0].DrawModel(*lod);
	Chair_md3[0].X=*x; 
	Chair_md3[0].Y=*y-0.3;
	Chair_md3[0].Z=-0.6;//*z;
	Chair_md3[0].D=1;//*d;
	Chair_md3[0].DrawModel(*lod);
	Chair_md3[0].X=*x-0.3; 
	Chair_md3[0].Y=*y;
	Chair_md3[0].D=3;//*d;
	Chair_md3[0].DrawModel(*lod);
	WoodenTable_md3[0].X=*x; 
	WoodenTable_md3[0].Y=*y;
	WoodenTable_md3[0].Z=-0.6;//*z;
	WoodenTable_md3[0].D=0;//*d;
	WoodenTable_md3[0].DrawModel(*lod);
}

extern "C" void DrawTable(float* x, float* y,int* lod){
	Table_md3[0].X=*x; 
	Table_md3[0].Y=*y;
	Table_md3[0].Z=-0.6;//*z;
	Table_md3[0].D=0;//*d;
	Table_md3[0].DrawModel(*lod);
}

extern "C" void DrawBed(float* x, float* y,int* lod){
	Bed_md3[0].X=*x; 
	Bed_md3[0].Y=*y;
	Bed_md3[0].Z=-0.6;//*z;
	Bed_md3[0].D=0;//*d;
	Bed_md3[0].DrawModel(*lod);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
//Treasures
///////////////////////////////////////////////////////////////////////////////////////////////////////////
extern "C" void DrawSilverKey(float* x, float* y,int* lod){
	Key_md3[0].X=*x; 
	Key_md3[0].Y=*y;
	Key_md3[0].Z=-0.6;//*z;
	Key_md3[0].D=0;//*d;
	Key_md3[0].DrawModel(*lod, 0);
}

extern "C" void DrawGoldenKey(float* x, float* y,int* lod){
	Key_md3[0].X=*x; 
	Key_md3[0].Y=*y;
	Key_md3[0].Z=-0.6;//*z;
	Key_md3[0].D=0;//*d;
	Key_md3[0].DrawModel(*lod, 1);
}

extern "C" void DrawCross(float* x, float* y,int* lod){
	Crosses_md3[0].X=*x; 
	Crosses_md3[0].Y=*y;
	Crosses_md3[0].Z=-0.6;//*z;
	Crosses_md3[0].D=0;//*d;
	Crosses_md3[0].DrawModel(*lod, 0);
}

extern "C" void DrawVase(float* x, float* y,int* lod){
	short int i=MapSMods[(int)*x][(int)*y];
	if (i>Vase_md3[0].Skins-1){
		//MessageBox(0, "", "Now", MB_OK);
		if (Vase_md3[0].Skins>3)i-=Vase_md3[0].Skins;i++;
		if (Vase_md3[0].Skins==3)i%=4;
		if (Vase_md3[0].Skins==2)i%=2;
		if (Vase_md3[0].Skins==1)i=0;
		MapSMods[(int)*x][(int)*y]=i;}
	Vase_md3[0].X=*x; 
	Vase_md3[0].Y=*y;
	Vase_md3[0].Z=-0.6;//*z;
	Vase_md3[0].D=0;//*d;
	Vase_md3[0].DrawModel(*lod,i);
}

extern "C" void DrawGoblet(float* x, float* y,int* lod){
	short int i=MapMods[(int)*x][(int)*y];
	if (i>Goblets-1){
		//MessageBox(0, "", "Now", MB_OK);
		if (Goblets>3)i-=Goblets;i++;
		if (Goblets==3)i%=4;
		if (Goblets==2)i%=2;
		if (Goblets==1)i=0;
		MapMods[(int)*x][(int)*y]=i;}
	//if (i>1) MessageBox(0, "", "Error", MB_OK);
	Goblets_md3[i].X=*x; 
	Goblets_md3[i].Y=*y;
	Goblets_md3[i].Z=-0.6;//*z;
	Goblets_md3[i].D=0;//*d;
	Goblets_md3[i].DrawModel(*lod, 0);
	/*
	Goblets_md3[1].X=*x; 
	Goblets_md3[1].Y=*y;
	Goblets_md3[1].Z=-0.6;//*z;
	Goblets_md3[1].D=0;//*d;
	Goblets_md3[1].DrawModel(*lod, 0);
/*	Goblets_md3[0].X=*x; 
	Goblets_md3[0].Y=*y;
	Goblets_md3[0].Z=-0.6;//*z;
	Goblets_md3[0].D=0;//*d;
	Goblets_md3[0].DrawModel(*lod, 0);*/
}

extern "C" void DrawTreasureBox(float* x, float* y,int* lod){
	TreasureBox_md3[0].X=*x; 
	TreasureBox_md3[0].Y=*y;
	TreasureBox_md3[0].Z=-0.6;//*z;
	TreasureBox_md3[0].D=0;//*d;
	TreasureBox_md3[0].DrawModel(*lod, 0);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
//Lights
///////////////////////////////////////////////////////////////////////////////////////////////////////////
extern "C" void DrawLamp(float* x, float* y,int* lod){
	Lamp_md3[0].X=*x; 
	Lamp_md3[0].Y=*y;
	Lamp_md3[0].Z=+0.3;//*z;
	Lamp_md3[0].D=0;//*d;
	Lamp_md3[0].DrawModel(*lod, 0);
}

extern "C" void DrawRedLamp(float* x, float* y,int* lod){
	Lamp_md3[0].X=*x; 
	Lamp_md3[0].Y=*y;
	Lamp_md3[0].Z=+0.3;//*z;
	Lamp_md3[0].D=0;//*d;
	Lamp_md3[0].DrawModel(*lod, 1);
}

extern "C" void DrawTorcher(float* x, float* y,int* lod){
	Torcher_md3[0].X=*x; 
	Torcher_md3[0].Y=*y;
	Torcher_md3[0].Z=-0.6;//*z;
	Torcher_md3[0].D=0;//*d;
	Torcher_md3[0].DrawModel(*lod);
}

extern "C" void DrawChandelier(float* x, float* y,int* lod){
glDisable(GL_DEPTH_TEST);
	Chandelier_md3[0].X=*x; 
	Chandelier_md3[0].Y=*y;
	Chandelier_md3[0].Z=-0.6;//*z;
	Chandelier_md3[0].D=0;//*d;
	Chandelier_md3[0].DrawModel(*lod);
	ChandelierGlass_md3[0].X=*x; 
	ChandelierGlass_md3[0].Y=*y;
	ChandelierGlass_md3[0].Z=-0.6;//*z;
	ChandelierGlass_md3[0].D=0;//*d;
	ChandelierGlass_md3[0].DrawModel(*lod);
glEnable(GL_DEPTH_TEST);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
//Plants
///////////////////////////////////////////////////////////////////////////////////////////////////////////
extern "C" void DrawRoots(float* x, float* y,int* lod){
	Roots_md3[0].X=*x; 
	Roots_md3[0].Y=*y;
	Roots_md3[0].Z=-0.4;//*z;
	Roots_md3[0].D=2*((int(*x)%2)==(int(*y)%2));//D=0 if x & y are even otherwise D=2
	// which gives the following orientation of roots:
	//     |-|-|-|-|-|-
	//     -|-|-|-|-|-|
	//     |-|-|-|-|-|-
	//     -|-|-|-|-|-|
	//     |-|-|-|-|-|-
	//     -|-|-|-|-|-|
	Roots_md3[0].DrawModel(*lod);
}

extern "C" void DrawRoundTree(float* x, float* y,int* lod){
	//glEnable(GL_CULL_FACE);								// Turn back face culling on
	//glCullFace(GL_FRONT);								// Quake3 uses front face culling apparently
	//culling
	//glCullFace(GL_FRONT);	
	//glEnable(GL_CULL_FACE);								// Turn back face culling on
	////glDisable(GL_CULL_FACE);
	RoundTree_md3[0].X=*x; 
	RoundTree_md3[0].Y=*y;
	RoundTree_md3[0].Z=-0.6;//*z;
	RoundTree_md3[0].D=0;

glDisable(GL_DEPTH_TEST);

	RoundTree_md3[0].DrawModel(*lod);

glEnable(GL_DEPTH_TEST);
	//glCullFace(GL_BACK);	
	//RoundTree_md3[0].DrawModel(*lod);
	//glEnable(GL_CULL_FACE);
	//glDisable(GL_CULL_FACE);
	//glCullFace(GL_BACK);//default
}

extern "C" void DrawVasedTree(float* x, float* y,int* lod){
	VasedTree_md3[0].X=*x; 
	VasedTree_md3[0].Y=*y;
	VasedTree_md3[0].Z=-0.6;//*z;
	VasedTree_md3[0].D=0;
	VasedTree_md3[0].DrawModel(*lod);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
//Knights
///////////////////////////////////////////////////////////////////////////////////////////////////////////
extern "C" void DrawKnight(float* x, float* y,int* lod){
	Sword_md3[0].X=*x; 
	Sword_md3[0].Y=*y;
	Sword_md3[0].Z=-0.6;//*z;
	Sword_md3[0].D=2;//*d;
	Sword_md3[0].DrawModel(*lod);
}

extern "C" void DrawSpears(float* x, float* y,int* lod){
	Holder_md3[0].X=*x; 
	Holder_md3[0].Y=*y;
	Holder_md3[0].Z=-0.6;//*z;
	Holder_md3[0].D=2;//*d;
	Holder_md3[0].DrawModel(*lod);
	/*for (int i=-0.25; i<0.25; i+=0.1){/ it hangs :-(
	Spears_md3[0].X=*x;
	Spears_md3[0].Y=*y+i; 
	Spears_md3[0].Z=-0.6;//*z;
	Spears_md3[0].D=2;//*d;
	Spears_md3[0].DrawModel(*lod);}*/
	Spears_md3[0].X=*x;
	Spears_md3[0].Y=*y-0.25; 
	Spears_md3[0].Z=-0.6;//*z;
	Spears_md3[0].D=2;//*d;
	Spears_md3[0].DrawModel(*lod);
	Spears_md3[0].Y=*y-0.15; 
	Spears_md3[0].DrawModel(*lod);
	Spears_md3[0].Y=*y-0.05; 
	Spears_md3[0].DrawModel(*lod);
	Spears_md3[0].Y=*y+0.05; 
	Spears_md3[0].DrawModel(*lod);
	Spears_md3[0].Y=*y+0.15; 
	Spears_md3[0].DrawModel(*lod);
	Spears_md3[0].Y=*y+0.25; 
	Spears_md3[0].DrawModel(*lod);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
//Remains
///////////////////////////////////////////////////////////////////////////////////////////////////////////
extern "C" void DrawLyingSkeleton(float* x, float* y,int* lod){
	LyingSkeleton_md3[0].X=*x; 
	LyingSkeleton_md3[0].Y=*y;
	LyingSkeleton_md3[0].Z=-0.5;//*z;
	LyingSkeleton_md3[0].D=0;
	LyingSkeleton_md3[0].DrawModel(*lod);
}

extern "C" void DrawWell(float* x, float* y,int* lod){
	Well_md3[0].X=*x; 
	Well_md3[0].Y=*y;
	Well_md3[0].Z=-0.6;//*z;
	Well_md3[0].D=0;
	Well_md3[0].DrawModel(*lod);
}

extern "C" void DrawWaterWell(float* x, float* y,int* lod){
	DrawWell(x, y, lod);
}

extern "C" void DrawCell(float* x, float* y, int* x_, int* y_, float* z,
								float* d/*direction*/,int* lod){
	CellDoor_md3[0].X=*x; 
	CellDoor_md3[0].Y=-*y;
	CellDoor_md3[0].Z=*z;
	CellDoor_md3[0].D=*d;
	CellDoor_md3[0].DrawModel(*lod);
}

extern "C" void DrawCelledSkeleton(float* x, float* y, int* x_, int* y_, float* z,
								float* d/*direction*/,int* lod){
	DrawCell(x, y, x_, y_, z, d, lod);
	CelledSkeleton_md3[0].X=*x; 
	CelledSkeleton_md3[0].Y=-*y;
	CelledSkeleton_md3[0].Z=*z;
	CelledSkeleton_md3[0].D=*d;
	CelledSkeleton_md3[0].DrawModel(*lod);
}






extern "C" void AddPortrait(int *x, int * y, int *state){//State for load only
	MapMD3s[*x][*y]=2;//;+-2**state;
Prt:MapMods[*x][*y]=floor(rand()*(5+1)/RAND_MAX);//kind of Portrait
	if (MapMods[*x][*y]>=5) goto Prt;

}

extern "C" void AddPoster(int *x, int * y, int *state){//State for load only
	MapMD3s[*x][*y]=3;//;+-2**state;
Pst:MapMods[*x][*y]=floor(rand()*(10+1)/RAND_MAX);//kind of Poster
	if (MapMods[*x][*y]>=10) goto Pst;
}

extern "C" int CheckMapModificator(int *x, int * y){
	return MapMods[*x][*y];
}


int MD3ModelsLoader(CModelMD3 Model[], const LPSTR ModelName, const LPSTR package,
					 const LPSTR path, bool multiple=false)
{bool bResult=true;
 //char *FullPath,//Fix it!!!!!!!!!!!
  //*ScaleConfig;
 char FullPath[512],//Fix it!!!!!!!!!!!
	 Config[512];
 char ModelNames[8][128]; int Models=0;
// LPSTR FullPath,//Fix it!!!!!!!!!!!
//	 ScaleConfig;
 int scales[3]={100,100,100};
	
	sprintf(FullPath,"\Models\\%s\\%s",package,path);
	//MessageBox(0, ModelName, "Now", MB_OK);
	if (multiple){int failed=0;
		//MessageBox(0, ModelName, "Multiple", MB_OK);
		//////////////////////////////////////////////////////////////////////////////////////////////////
		//Reading Models List
		//////////////////////////////////////////////////////////////////////////////////////////////////
		sprintf(Config,"\Models\\%s\\%s\\%s.list",package,path,ModelName);
		// Open the config file
		ifstream fin(Config);
		//parsing Scales Config
		string strWord = "";				// This stores the current word we are reading in
		string strLine = "";				// This stores the current line we read in
		if(!fin.fail()){while( fin >> strWord){
			// If the first character of the word is NOT a number, we haven't hit an animation line
			if(!isdigit(strWord[0])){// Store the rest of this line and go to the next one
				getline(fin, strLine);
				continue;}
			//up to 8 models for each object
			fin >> ModelNames[Models];Models++;if(Models>7)break;
		}
			fin.close();}
		//for (int i; i<Models; i++){//it was a bug but it was working somehow......
		for (int i=0; i<Models; i++){
			////////////////////////////////////////////////////////////////////////////////////////////////
			//Reading Scales
			////////////////////////////////////////////////////////////////////////////////////////////////
			sprintf(Config,"\Models\\%s\\%s\\%s.scale",package,path,ModelNames[i]);
			// Open the config file
			ifstream fin(Config);
			//parsing Scales Config
			string strWord = "";				// This stores the current word we are reading in
			string strLine = "";				// This stores the current line we read in
			if(!fin.fail()){while( fin >> strWord){
				// If the first character of the word is NOT a number, we haven't hit an animation line
				if(!isdigit(strWord[0])){// Store the rest of this line and go to the next one
					getline(fin, strLine);
					continue;}
				scales[0]=atoi(strWord.c_str()); fin >> scales[1] >> scales[2];}
				fin.close();}			
			////////////////////////////////////////////////////////////////////////////////////////////////
			//Loading Models
			////////////////////////////////////////////////////////////////////////////////////////////////
			//MessageBox(0, ModelNames[i-failed], "Model", MB_OK);
			//MessageBox(0, ModelNames[i-failed], "Model", MB_OK);
			bResult=Model[i-failed].LoadModel(FullPath, ModelNames[i-failed],"");
			//bResult=Model[0].LoadModel(FullPath, ModelName,skin);
			if (!bResult){ failed++;}
			Model[i-failed].ScaleX=scales[0];Model[i-failed].ScaleY=scales[1];
			Model[i-failed].ScaleZ=scales[2];}
			return Models-failed;//how many models have been successfully loaded
	}else{
		//MessageBox(0, ModelName, "Single", MB_OK);
		//////////////////////////////////////////////////////////////////////////////////////////////////
		//Reading Scales	
		//////////////////////////////////////////////////////////////////////////////////////////////////
		sprintf(Config,"\Models\\%s\\%s\\%s.scale",package,path,ModelName);
		// Open the config file
		ifstream fin(Config);
		//parsing Scales Config
		string strWord = "";				// This stores the current word we are reading in
		string strLine = "";				// This stores the current line we read in
		if(!fin.fail()){while( fin >> strWord){
			// If the first character of the word is NOT a number, we haven't hit an animation line
			if(!isdigit(strWord[0])){// Store the rest of this line and go to the next one
				getline(fin, strLine);
				continue;}
		scales[0]=atoi(strWord.c_str()); fin >> scales[1] >> scales[2];}
		fin.close();}
		//////////////////////////////////////////////////////////////////////////////////////////////////
		//Loading Models
		//////////////////////////////////////////////////////////////////////////////////////////////////
		bResult=Model[0].LoadModel(FullPath, ModelName,"");
		if (!bResult) return false;//if a model cannot be load return false
		Model[0].ScaleX=scales[0];Model[0].ScaleY=scales[1];Model[0].ScaleZ=scales[2];
		return true;}
}


void InitMD3Models(void)
{
	int Result;

	//Load Architectonics
		//Iron-Eagle
	Result=MD3ModelsLoader(Alcove_md3,  "Alcove", "Architectonic.pk3", "IronEagle");
	Result=MD3ModelsLoader(Eagle_md3, "Eagle",	"Architectonic.pk3", "IronEagle");
	Result=MD3ModelsLoader(Eagle_md3_, "Eagle", "Architectonic.pk3", "IronEagle");
		//now we shood floor all mid & low-res fallen eagles
/*		t3DModel *pModel;//Fix!!!!!!!!!!!!!!!!!!!
		pModel = Eagle_md3_[1].GetModel(0);
		pModel->currentAnim=1;
		pModel = Eagle_md3_[0].GetModel(0);
		pModel->currentAnim=1;*/
		//Columns
	Result=MD3ModelsLoader(Column_md3, "Column", "Architectonic.pk3", "Column");
	//, "_White"


	//Load Furniture
		//Table & Chairs
	Result=MD3ModelsLoader(Chair_md3, "Chair", "Furniture.pk3", "Chairs&Table");
	Result=MD3ModelsLoader(WoodenTable_md3, "Table", "Furniture.pk3", "Chairs&Table");
		//Table
	Result=MD3ModelsLoader(Table_md3, "Table", "Furniture.pk3", "Table");
		//Bed
	Result=MD3ModelsLoader(Bed_md3, "Bed", "Furniture.pk3", "Bed");
		

	//Lights
		//Lamp
	Result=MD3ModelsLoader(Lamp_md3, "Lamp", "Lights.pk3", "Lamp");
	Result=MD3ModelsLoader(Chandelier_md3, "Chandelier", "Lights.pk3", "Chandelier");
	Result=MD3ModelsLoader(ChandelierGlass_md3, "Chandelier_Glass", "Lights.pk3", "Chandelier");
	Result=MD3ModelsLoader(Torcher_md3, "Torcher", "Lights.pk3", "Torcher");

	//Knights
	Result=MD3ModelsLoader(Sword_md3, "GothicSword", "Knights.pk3", "Swords");
	Result=MD3ModelsLoader(Holder_md3, "Holder", "Knights.pk3", "PoleArms");
	Result=MD3ModelsLoader(Spears_md3, "Spear", "Knights.pk3", "PoleArms");
	//Result=MD3ModelsLoader(Sword_md3, "GothicSword", "Knights.pk3", "Swords", true);

	//Plants
	Result=MD3ModelsLoader(Roots_md3, "Roots", "Plants.pk3", "Roots");
	Result=MD3ModelsLoader(RoundTree_md3, "RoundTree", "Plants.pk3", "Trees");
	Result=MD3ModelsLoader(VasedTree_md3, "VasedTree", "Plants.pk3", "Trees");

	//Remains
	Result=MD3ModelsLoader(LyingSkeleton_md3, "Lying", "Remains.pk3", "Skeletons");
	Result=MD3ModelsLoader(Well_md3, "Well", "Remains.pk3", "Well");
	Result=MD3ModelsLoader(CellDoor_md3, "Door", "Remains.pk3", "Skeletons");
	Result=MD3ModelsLoader(CelledSkeleton_md3, "Celled", "Remains.pk3", "Skeletons");

	//Treasure
		//Key
	Result=MD3ModelsLoader(Key_md3, "Key", "Treasures.pk3", "Key");
		//Crosses
	Crosses=MD3ModelsLoader(Crosses_md3, "Cross", "Treasures.pk3", "Crosses");
		//Goblets
	Goblets=MD3ModelsLoader(Goblets_md3, "Goblet", "Treasures.pk3", "Goblets", true);
		//Goblets
	Result=MD3ModelsLoader(Vase_md3, "Vase", "Treasures.pk3", "Vases");
	Result=MD3ModelsLoader(TreasureBox_md3, "Treasure_Box", "Treasures.pk3", "TreasureBox");

	// Load the gun and attach it to our character
//	bResult = g_Model.LoadWeapon(MODEL_PATH, GUN_NAME);


//////////// *** NEW *** ////////// *** NEW *** ///////////// *** NEW *** ////////////////////

	// When we get here, the character should have everything loaded.  Before going on,
	// we want to set the current animation for the torso and the legs.

	// Set the standing animation for the torso
	/*g_Model.SetTorsoAnimation("TORSO_STAND");

	// Set the walking animation for the legs
	g_Model.SetLegsAnimation("LEGS_WALK");

//////////// *** NEW *** ////////// *** NEW *** ///////////// *** NEW *** ////////////////////

	glEnable(GL_CULL_FACE);								// Turn back face culling on
	glCullFace(GL_FRONT);								// Quake3 uses front face culling apparently

	glEnable(GL_TEXTURE_2D);							// Enables Texture Mapping
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing*/
//	glEnable(GL_CULL_FACE);								// Turn back face culling on
//	glCullFace(GL_FRONT);								// Quake3 uses front face culling apparently

//	glEnable(GL_TEXTURE_2D);							// Enables Texture Mapping
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
}









/*
//////////// *** NEW *** ////////// *** NEW *** ///////////// *** NEW *** ////////////////////

// This tutorial is the second MD3 tutorial.  In this version we will be able to load and
// animate the Quake3 character using quaternions.    Be sure you have read the MD3 Loader
// and Quaternions tutorial on our site at www.GameTutorials.com in the OpenGL section.
// This tutorial was written assuming you did, which enables us to cut out a ton of comments
// and explanations that can be found in the previous tutorials.  I will leave some comments
// to refresh your memory, but most of the large blocks will be taken out.
// 
// Okay, so what's different from this tutorial and the last one?  Well, if you run the
// program you will see that our character is animating.  If you click the right and let
// mouse buttons you can cycle through the torso and leg animations.  The name of the
// animations are also displayed in the title bar.  So the question is, how much of the
// code changed to make this happen?  Unfortunately, it's just a couple simple additions,
// but it isn't a bunch of complicated ones either.  The only complicated code is the
// quaternion code.  As I explained in the first quaternions tutorial, the math is quite
// complex to wrap your head around.  Don't stress out about trying to understand why it works,
// it's sufficient enough to understand the equations needed to perform the desired task.
// It goes without saying that if you do have a desire to delve into the math and proofs,
// you will be a better person for it, for it can only help you.  I will NOT be going over
// the theory or proofs beneath the quaternion equations.  Just get a math book.  I will
// talk more about the quaternions in Md3.cpp.  Here is a address to a really detailed PDF 
// file written by Ken Shoemake on quaternions:
//
// http://www.daimi.au.dk/~mbl/cgcourse/wiki/_files/shoemake94.pdf
//
// As well as another great PDF from Magic Software here:
//
// http://www.magic-software.com/Documentation/quat.pdf
//
// One of the most notable changes added to this tutorial is that we now read in the
// animation config file, with the .cfg extension.  Remember from the last tutorial that
// only the torso (upper) and the legs (lower) have animation associated with them.  If you
// want the head to animate, the artist will need to make the head part of the upper.md3 file.
// This is why on some models you will see a small head when you turn on wire frame mode.
// The config file stores all of the animations and their respective frame data.  For example, 
// here is a few lines from a config file:
//
//		0	31	0	25		// BOTH_DEATH1
//		31	1	1	25		// BOTH_DEAD1
//		32	35	0	20		// BOTH_DEATH2
//
// These 3 lines indicate 3 animations that both the torso and legs must perform.
// The four numbers in front of the animation name store the starting frame, the ending
// frame, the number of looping frames (ignored) and the frames per second the animation runs.
// In the upper.md3 and the lower.md3, vertex key frames are saved, almost exactly like the
// .MD2 file format is saved.  The first 2 numbers are indices into those key frames, which
// are loading with the vertex array.  They aren't actually indexed directly, but depending
// on the current key frame, you would say:
//
// currentIndex = pModel->currentFrame * pObject->numOfVerts;
//
// Since all the key frames are stored in a 1 dimensional array, we treat it like a 2D
// array by multiplying the current frame by the number of vertices of the model (which
// does not count the key frames, just vertex count for one frame).  That new index is
// used to index into the vertex array of the current model.
//
// There are 3 blocks of animations in the config file.  They are BOTH_*, TORSO_* and LEGS_*.
// This is because the torso model can be doing a different animation that has nothing to do
// with the legs animation.  For instance, the character can be doing a shooting animation,
// while they are jumping, landing, turning, swimming, etc...
//
// It is assumed that by reading this tutorial you understand a decent amount of OpenGL and
// are comfortable with advanced topics.  This is a extremely advanced topic and should not
// be tackled until you get to a certain level of understanding in 3D.  I would recommend
// people who might not be up to that level to try looking at the MD2 Loader and animation.
// This is still an advanced tutorial, but not as complicated.  It only gets harder from here.
// And remember, copy and pasting of code hurts small children in Africa :)
//
// The last notable change from the last tutorial is that we now use a matrix to perform the
// translations and rotations of our model's body parts for animation.  Since we are using 
// quaternions, and the fact that the key frame rotations are stored in a 3x3 matrix. this 
// is necessary.  The rest of the functions and things added are just smaller helper functions.
//
// Before we get to the code, I would like to remind those who might be lost, to go back to
// the original MD3 Loader tutorial and see if there is an explanation that you missed and was
// taken out of this version.
// 
// The controls for this application are:
//
// Right Mouse Click - Cycles through the upper (torso) animations
// Left Mouse Click  - Cycles through the lower (legs) animations		
// Left Arrow Key - Spins the model to the left
// Right Arrow Key - Spins the model to the right
// Up Arrow Key - Moves the camera closer to the model
// Right Arrow Key - Moves the camera farther away from the model
// W - Changes the Render mode from normal to wire frame.
// Escape - Quits
//
// * Note * Keep in mind that when you change animations it is no immediately.  It waits to
// finish the current animation that it's on.  This can be changed easily if you read 
// the note in IncreaseCharacterAnimation() below.
//
//

//////////// *** NEW *** ////////// *** NEW *** ///////////// *** NEW *** ////////////////////

#define MODEL_PATH  "Models"				// This stores the folder name the character is in
#define MODEL_NAME	"Eagle_hi"				// This stores the prefix for all the character files
//#define GUN_NAME    "Railgun"			// This stores the name of our gun to load

bool  g_bFullScreen = true;				// Set full screen as default
HWND  g_hWnd;							// This is the handle for the window
RECT  g_rRect;							// This holds the window dimensions
HDC   g_hDC;							// General HDC - (handle to device context)
HGLRC g_hRC;							// General OpenGL_DC - Our Rendering Context for OpenGL
HINSTANCE g_hInstance;					// This holds the global hInstance for UnregisterClass() in DeInit()

UINT g_Texture[MAX_TEXTURES] = {0};		// This holds the texture info, referenced by an ID

CModelMD3 g_Model;						// This is our global md3 character model instance


float g_RotateX		  =    0.0f;		// This is the current value at which the model is rotated
float g_RotationSpeed =    0.1f;		// This is the speed that our model rotates. 
float g_TranslationX = 0.f;
float g_TranslationY = 150.0f;
float g_TranslationZ  = -500.0f;		// This stores our distance away from the model

bool  g_RenderMode    =    true;		// This tells us if we are in wire frame mode or not


///////////////////////////////// INIT GAME WINDOW \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This function initializes the game window.
/////
///////////////////////////////// INIT GAME WINDOW \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void Init(HWND hWnd)
{
	g_hWnd = hWnd;										// Assign the window handle to a global window handle
	GetClientRect(g_hWnd, &g_rRect);					// Assign the windows rectangle to a global RECT
	InitializeOpenGL(g_rRect.right, g_rRect.bottom);	// Init OpenGL with the global rect

	// Load the 3 body part meshes and their skin, textures and animation config files
	bool bResult = g_Model.LoadModel(MODEL_PATH, MODEL_NAME);

	// Load the gun and attach it to our character
//	bResult = g_Model.LoadWeapon(MODEL_PATH, GUN_NAME);


//////////// *** NEW *** ////////// *** NEW *** ///////////// *** NEW *** ////////////////////

	// When we get here, the character should have everything loaded.  Before going on,
	// we want to set the current animation for the torso and the legs.

	// Set the standing animation for the torso
	g_Model.SetTorsoAnimation("TORSO_STAND");

	// Set the walking animation for the legs
	g_Model.SetLegsAnimation("LEGS_WALK");

//////////// *** NEW *** ////////// *** NEW *** ///////////// *** NEW *** ////////////////////

	glEnable(GL_CULL_FACE);								// Turn back face culling on
	glCullFace(GL_FRONT);								// Quake3 uses front face culling apparently

	glEnable(GL_TEXTURE_2D);							// Enables Texture Mapping
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
}


///////////////////////////////// MAIN GAME LOOP \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This function Handles the main game loop
/////
///////////////////////////////// MAIN GAME LOOP \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

WPARAM MainLoop()
{
	MSG msg;

	while(1)											// Do our infinate loop
	{													// Check if there was a message
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) 
        { 
			if(msg.message == WM_QUIT)					// If the message wasnt to quit
				break;
            TranslateMessage(&msg);						// Find out what the message does
            DispatchMessage(&msg);						// Execute the message
        }
		else											// if there wasn't a message
		{ 
			RenderScene();								// Update the screen	
        } 
	}

	DeInit();											// Clean up and free all allocated memory

	return(msg.wParam);									// Return from the program
}


///////////////////////////////// RENDER SCENE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This function renders the entire scene.
/////
///////////////////////////////// RENDER SCENE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void RenderScene() 
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer
	glLoadIdentity();									// Reset The matrix

	// Notice that nothing has changed in the rendering, it's still the same (* NEW *)

	// Give OpenGL our position,			then view,			then up vector
	//gluLookAt(	0, 5.5f, g_TranslationZ,	0, 5.5f, 0,			0, 1, 0);
	gluLookAt(	g_TranslationX, g_TranslationY, g_TranslationZ,	0, g_TranslationY, 0,
		0, 1, 0);	
	// We want the model to rotate around the axis so we give it a rotation
	// value, then increase/decrease it. You can rotate right or left with the arrow keys.

	glRotatef(g_RotateX, 0, 1.0f, 0);			// Rotate the object around the Y-Axis
	g_RotateX += g_RotationSpeed;				// Increase the speed of rotation

	g_Model.DrawModel();						//  Render our character to the screen

	SwapBuffers(g_hDC);							// Swap the backbuffers to the foreground
}


//////////// *** NEW *** ////////// *** NEW *** ///////////// *** NEW *** ////////////////////

////////////////////////// INCREASE CHARACTER ANIMATION \\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This function increases the model's torso or legs animation
/////
////////////////////////// INCREASE CHARACTER ANIMATION \\\\\\\\\\\\\\\\\\\\\\\\\\*

void IncreaseCharacterAnimation(CModelMD3 *pCharacter, int whichPart)
{
	char strWindowTitle[255] = {0};
	t3DModel *pModel, *pUpper, *pLower;

	// This function doesn't have much to do with the character animation, but I
	// created it so that we can cycle through each of the animations to see how
	// they all look.  You can press the right and left mouse buttons to cycle through
	// the torso and leg animations.  If the current animation is the end animation,
	// it cycles back to the first animation.  This function takes the character you
	// want, then the define (kLower, kUpper) that tells which part to change.

	// Here we store pointers to the legs and torso, so we can display their current anim name
	pLower = pCharacter->GetModel(kLower);
	pUpper = pCharacter->GetModel(kUpper);

	// This line gives us a pointer to the model that we want to change
	pModel = pCharacter->GetModel(whichPart);

	// To cycle through the animations, we just increase the model's current animation
	// by 1.  You'll notice that we also mod this result by the total number of
	// animations in our model, to make sure we go back to the beginning once we reach
	// the end of our animation list.  

	// Increase the current animation and mod it by the max animations
	pModel->currentAnim = (pModel->currentAnim + 1) % (pModel->numOfAnimations);

	// Set the current frame to be the starting frame of the new animation
	pModel->nextFrame = pModel->pAnimations[pModel->currentAnim].startFrame;

	// (* NOTE *) Currently when changing animations, the character doesn't immediately
	// change to the next animation, but waits till it finishes the current animation
	// and slowly blends into the next one.  If you want an immediate switch, change
	// the pModel-nextFrame to pModel->currentFrame.

	// Display the current animations in our window's title bar
	sprintf(strWindowTitle, "Eagle Animation Test - Md3 Animation: %s", 
			pLower->pAnimations[pLower->currentAnim].strName, 
			pUpper->pAnimations[pUpper->currentAnim].strName);

	// Set the window's title bar to our new string of animation names
	SetWindowText(g_hWnd, strWindowTitle);
}

//////////// *** NEW *** ////////// *** NEW *** ///////////// *** NEW *** ////////////////////


///////////////////////////////// WIN PROC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This function handles the window messages.
/////
///////////////////////////////// WIN PROC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

LRESULT CALLBACK WinProc(HWND hWnd,UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    LONG    lRet = 0; 
    PAINTSTRUCT    ps;

	switch (uMsg)
	{ 
    case WM_SIZE:								// If the window is resized
		if(!g_bFullScreen)						// Do this only if we are NOT in full screen
		{
			SizeOpenGLScreen(LOWORD(lParam),HIWORD(lParam));// LoWord=Width, HiWord=Height
			GetClientRect(hWnd, &g_rRect);		// Get the window rectangle
		}
        break; 
 
	case WM_PAINT:								// If we need to repaint the scene
		BeginPaint(hWnd, &ps);					// Init the paint struct		
		EndPaint(hWnd, &ps);					// EndPaint, Clean up
		break;

	// Below we define our controls for this tutorial. The controls are: 

	// Right Mouse Click - Cycles through the upper (torso) animations
	// Left Mouse Click  - Cycles through the lower (legs) animations	
	// Left Arrow Key - Spins the model to the left
	// Right Arrow Key - Spins the model to the right
	// Up Arrow Key - Moves the camera closer to the model
	// Right Arrow Key - Moves the camera farther away from the model
	// W - Changes the Render mode from normal to wire frame.
	// Escape - Quits


//////////// *** NEW *** ////////// *** NEW *** ///////////// *** NEW *** ////////////////////

	case WM_LBUTTONDOWN:								// If the left mouse button was clicked
		
		// Increase the leg's current animation to the next animation
		IncreaseCharacterAnimation( &g_Model, kLower);
		break;

	case WM_RBUTTONDOWN:								// If the right mouse button was clicked

		// Increase the torso's current animation to the next animation
		IncreaseCharacterAnimation( &g_Model, kUpper );
		break;

//////////// *** NEW *** ////////// *** NEW *** ///////////// *** NEW *** ////////////////////


	case WM_KEYDOWN:							// If we pressed a key

		switch(wParam)							// Check if we hit a key
		{
			case VK_ESCAPE:						// If we hit the escape key
				PostQuitMessage(0);				// Send a QUIT message to the window
				break;

			case VK_LEFT:						// If the LEFT arrow key was pressed
				g_RotationSpeed -= 0.05f;		// Decrease the rotation speed (eventually rotates left)
				break;

			case VK_RIGHT:						// If the RIGHT arrow key is pressed
				g_RotationSpeed += 0.05f;		// Increase the rotation speed (rotates right)
				break;

			case VK_UP:							// If the UP arrow key was pressed
				g_TranslationZ += 2;			// Move the camera position forward along the Z axis
				break;

			case VK_DOWN:						// If the DOWN arrow key is pressed
				g_TranslationZ -= 2;			// Move the camera position back along the Z axis
				break;

			case 'W':

				g_RenderMode = !g_RenderMode;	// Change the rendering mode

				// Change the rendering mode to and from lines or triangles
				if(g_RenderMode) 				
				{
					// Render the triangles in fill mode		
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);	
				}
				else 
				{
					// Render the triangles in wire frame mode
					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);	
				}
				break;
		}
		break;

    case WM_CLOSE:										// If the window is being closed
        PostQuitMessage(0);								// Send a QUIT Message to the window
        break; 
     
    default:											// Return by default
        lRet = DefWindowProc (hWnd, uMsg, wParam, lParam); 
        break; 
    } 
 
    return lRet;										// Return by default
}

/////////////////////////////////////////////////////////////////////////////////
//
// * QUICK NOTES * 
// 
// You probably noticed that there wasn't much changed in this file.  Most of the
// changes happened in Md3.cpp.  Since I wrote this tutorial before writing the
// previous tutorial, I was able to eliminate tons of little changes, due to the
// differences that would have been added with the animation.  That way you don't
// need to look at a bunch of changes as much as the additions of code.
// 
// Below is a excerpt from Md3.cpp explaining more additions to the previous tutorial:
// 
// -----
//
// First of all, we added a basic quaternion class to our tutorial.  This is used
// to take a matrix, convert it to a quaternion, interpolate between another 
// quaternion that was converted to a matrix, then turned back into a matrix.
// This is because quaternions are a great way to interpolate between rotations.
// If you wanted to use the glRotatef() and glTranslatef() functions, you would
// want to convert the interpolated quaternion to an axis angle representation,
// which might be less code, but not as useful in the long run.
// 
// The next important thing that was added was the interpolation between vertex
// key frames.  We learned earlier (top of Md3.cpp) that not most of the animation
// is done through key frame vertices, not bones.  The only bone animation that is
// done is with the joints that connect the .md3 models together.  Half Life, for
// example, uses full on skeletal animation.
// 
// Also, in this tutorial we added the code to parse the animation config file (.cfg).
// this stores the animation information for each animation.  The order of the are
// important.  For the most part, the config files are the same format.  As discussed
// in the previous MD3 tutorial, there is a few extra things in the config file that
// we don't read in here, such as footstep sounds and initial positions.  The tutorial
// was not designed to be a robust reusable Quake3 character loader, but to teach you
// how it works for the most part.  Other things are extra credit :)
//
// Another important thing was our timing system.  Since we were dealing with multiple
// models that had different frames per second, we needed to add some variables to our
// t3DModel class to hold some things like elapsedTime and the current t value.  This
// way, no static variables had to be created, like in the .MD2 tutorial.
//
// ----- 
//
// Be sure to check out the top of Md3.cpp for a more detailed explanation of animation
// and interpolation.
// 
//
// Let me know if this helps you out!
// 
// 
// Ben Humphrey (DigiBen)
// Game Programmer
// DigiBen@GameTutorials.com
// Co-Web Host of www.GameTutorials.com
//
// The Quake2 .Md3 file format is owned by ID Software.  This tutorial is being used 
// as a teaching tool to help understand model loading and animation.  This should
// not be sold or used under any way for commercial use with out written consent
// from ID Software.
//
// Quake, Quake2 and Quake3 are trademarks of ID Software.
// Lara Croft is a trademark of Eidos and should not be used for any commercial gain.
// All trademarks used are properties of their respective owners. 
//
//
*/