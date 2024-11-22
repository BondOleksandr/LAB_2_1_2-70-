#include <raylib.h>
#include <iostream>
#include <string>
#include <random>

using namespace std;

struct controls{
    bool pause=true;
    bool dir=true;
    int framerate=60;
    int walk;
    int trck=0;
    int vall;
    bool selection = false;
};

typedef struct BSNode {
    int* val;
    int step;
    int Aelem;
    int Belem;
    Color ord;
    bool fin = false;
    BSNode* next;
    BSNode* prev;
}BSListn, * BSListnp;

typedef struct ISNode {
    int* val;
    int step;
    int Aelem;
    int Belem;
    int keyelem;
    Color aord;
    Color bord;
    ISNode* next;
    ISNode* prev;
}ISListn, * ISListnp;

typedef struct MMNode {
    int** Rval;
    int I=0;
    int J=0;
    int K=0;
    int stp=0;
    MMNode* next = NULL;
    MMNode* prev =NULL;
}MMListn, * MMListnp;

BSListnp BSsave = NULL;
ISListnp ISsave = NULL;
MMListnp MMsave = NULL;

class object {
public:
    virtual void Draw() const = 0;
    virtual ~object() = default;
};

class Text : public object {
private:
    const char* txt;
    int X;
    int Y;
    int FSize;
    Color CLR;

public:
    Text(const char TXT[], int x, int y, int fsize, Color clr): txt(TXT), X(x), Y(y), FSize(fsize), CLR(clr){};

    const char* getTxt() const { return txt; }
    int getX() const { return X; }
    int getY() const { return Y; }
    int getFSize() const { return FSize; }
    Color getColor() const { return CLR; }

    void setTxt(const char* TXT) { txt = TXT; }
    void setX(int x) { X = x; }
    void setY(int y) { Y = y; }
    void setFSize(int fsize) { FSize = fsize; }
    void setColor(Color clr) { CLR = clr; }

    void Draw() const override {
        DrawText(txt, X, Y, FSize, CLR);
    }
};

class RTangle : public object {
private:
    int X=0;
    int Y=0;
    int LGT=0;
    int HGT=0;
    Color CLR=RAYWHITE;

public:
    RTangle(int x, int y, int lgt, int hgt, Color clr) : X(x), Y(y), LGT(lgt), HGT(hgt), CLR(clr) {}

    int getX() const { return X; }
    int getY() const { return Y; }
    int getLength() const { return LGT; }
    int getHeight() const { return HGT; }
    Color getColor() const { return CLR; }

    void setX(int x) { X = x; }
    void setY(int y) { Y = y; }
    void setLength(int lgt) { LGT = lgt; }
    void setHeight(int hgt) { HGT = hgt; }
    void setColor(Color clr) { CLR = clr; }

    void Draw() const override {
        DrawRectangle(X, Y, LGT, HGT, CLR);
    }
};

RTangle PAUSEdefRT(20, 750,200,100, DARKBLUE);
Text PAUSEdefTXT("PAUSE", 38,777,50, BLACK);

RTangle CHNGDIRdefRT(240, 750,200,100, RED);
Text CHNGDIRdefTXT("RVS", 290,777,50, WHITE);

RTangle FPSINCdefRT(460, 750,200,100, ORANGE);
Text FPSINCdefTXT("FPS+", 510,777,50, WHITE);

RTangle FPSDECdefRT(680, 750,200,100, BLUE);
Text FPSDECdefTXT("FPS-", 725,777,50, WHITE);

RTangle WLKRdefRT(1120, 750,200,100, GREEN);
Text WLKRdefTXT(">>>>", 1155,777,50, WHITE);

RTangle WLKLdefRT(900, 750,200,100, GREEN);
Text WLKLdefTXT("<<<<", 940,777,50, WHITE);

RTangle SeldefRT(1340, 750,200,100, PURPLE);
Text SeldefTXT("SEL", 1370,777,50, WHITE);
////////////////////////////////////////////////////////////////////////////

RTangle mARRdefRT(10, 10,580,100, PURPLE);
Text mARRdefTXT("Array sortings", 30,37,50, WHITE);

RTangle mMATdefRT(10, 220,580,100, PURPLE);
Text mMATdefTXT("Matrix multiplication", 30,257,50, WHITE);

RTangle mHOMEdefRT(10, 430,580,100, GREEN);
Text mHOMEdefTXT("Main menu", 30,477,50, BLACK);

RTangle mARRBSdefRT(10, 10,580,100, PURPLE);
Text mARRBSdefTXT("Bubble sort", 30,37,50, WHITE);

RTangle mARRINSdefRT(10, 220,580,100, PURPLE);
Text mARRINSdefTXT("Insertion sort", 30,257,50, WHITE);

RTangle mARRNEWdefRT(10, 10,580,100, PURPLE);
Text mARRNEWdefTXT("New array", 30,37,50, WHITE);

RTangle mSAVEdefRT(10, 220,580,100, PURPLE);
Text mSAVEdefTXT("Load save", 30,257,50, WHITE);

RTangle mMATNEWdefRT(10, 10,580,100, PURPLE);
Text mMATNEWdefTXT("New matrix", 30,37,50, WHITE);


RTangle mDEFRT[8]={mARRdefRT, mMATdefRT, mHOMEdefRT, mARRBSdefRT, mARRINSdefRT, mARRNEWdefRT, mSAVEdefRT,mMATNEWdefRT};
Text mDEFTXT[8]={mARRdefTXT, mMATdefTXT, mHOMEdefTXT, mARRBSdefTXT, mARRINSdefTXT, mARRNEWdefTXT,mSAVEdefTXT,mMATNEWdefTXT};

RTangle DEFRT[7]={PAUSEdefRT, CHNGDIRdefRT, FPSINCdefRT, FPSDECdefRT, WLKRdefRT, WLKLdefRT, SeldefRT};
Text DEFTXT[7]={PAUSEdefTXT, CHNGDIRdefTXT, FPSINCdefTXT, FPSDECdefTXT, WLKRdefTXT, WLKLdefTXT, SeldefTXT};

class CMNBUTTON {
protected:
    RTangle btn;
    Text inf;

    bool IsButtonClicked(Vector2 mousePosition) const {
        Rectangle check = {btn.getX(), btn.getY(), btn.getLength(), btn.getHeight()};
        return CheckCollisionPointRec(mousePosition, check) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
    }

public:
    CMNBUTTON(RTangle BTN, Text INF) : btn(BTN), inf(INF) {}

    virtual ~CMNBUTTON() = default;

    virtual controls evaluate(controls A) = 0;

    virtual void Draw() {
        btn.Draw();
        inf.Draw();
    }

    void SetButton(const RTangle& newBtn, const Text& newInf) {
        btn = newBtn;
        inf = newInf;
    }
};

class PauseButton : public CMNBUTTON {
public:
    PauseButton(RTangle BTN, Text INF) : CMNBUTTON(BTN, INF) {}
    controls evaluate(controls BS) override {
        if (IsButtonClicked(GetMousePosition())) {
            if (BS.pause) {
                BS.pause = false;
                btn.setColor(DARKGRAY);
            } else {
                BS.pause = true;
                btn.setColor(DARKBLUE);
            }
        }
        Draw();
        return BS;
    }
};

class DirrchangeButton : public CMNBUTTON {
public:
    DirrchangeButton(RTangle BTN, Text INF) : CMNBUTTON(BTN, INF) {}
    controls evaluate(controls BS) override {
        if (IsButtonClicked(GetMousePosition())) {
            btn.setColor(ORANGE);
            if (BS.dir) {
                inf.setTxt("FWD");
                BS.dir = false;
            } else {
                inf.setTxt("RVS");
                BS.dir = true;
            }
        }
        Draw();
        btn.setColor(RED);
        return BS;
    }
};

class FPSincButton : public CMNBUTTON {
public:
    FPSincButton(RTangle BTN, Text INF) : CMNBUTTON(BTN, INF) {}
    controls evaluate(controls BS) override {
        if (IsButtonClicked(GetMousePosition())) {
            btn.setColor(BLUE);
            BS.framerate += 2;
        }
        Draw();
        btn.setColor(ORANGE);
        return BS;
    }
};

class FPSdecButton : public CMNBUTTON {
public:
    FPSdecButton(RTangle BTN, Text INF) : CMNBUTTON(BTN, INF) {}
    controls evaluate(controls BS) override {
        if (IsButtonClicked(GetMousePosition()) && BS.framerate > 2) {
            btn.setColor(ORANGE);
            BS.framerate -= 2;
        }
        Draw();
        btn.setColor(BLUE);
        return BS;
    }
};

class WLKrButton : public CMNBUTTON {
public:
    WLKrButton(RTangle BTN, Text INF) : CMNBUTTON(BTN, INF) {}
    controls evaluate(controls BS) override {
        if (IsButtonClicked(GetMousePosition())) {
            btn.setColor(PURPLE);
            BS.walk = 1;
            BS.pause = true;
        }
        Draw();
        btn.setColor(GREEN);
        return BS;
    }
};

class WLKlButton : public CMNBUTTON {
public:
    WLKlButton(RTangle BTN, Text INF) : CMNBUTTON(BTN, INF) {}
    controls evaluate(controls BS) override {
        if (IsButtonClicked(GetMousePosition())) {
            btn.setColor(PURPLE);
            BS.walk = -1;
            BS.pause = true;
        }
        Draw();
        btn.setColor(GREEN);
        return BS;
    }
};

class SELButton : public CMNBUTTON {
public:
    SELButton(RTangle BTN, Text INF) : CMNBUTTON(BTN, INF) {}
    controls evaluate(controls BS) override {
        if (IsButtonClicked(GetMousePosition())) {
            if (!BS.selection) {
                btn.setColor(YELLOW);
                BS.pause = true;
                BS.selection = true;
            } else {
                BS.pause = true;
                BS.selection = false;
                btn.setColor(PURPLE);
            }
        }
        Draw();
        return BS;
    }
};


class MENUBUTTON {
protected:
    RTangle btn;
    Text inf;

    bool IsButtonClicked(Vector2 mousePosition) const {
        Rectangle check = {btn.getX(), btn.getY(), btn.getLength(), btn.getHeight()};
        return CheckCollisionPointRec(mousePosition, check) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
    }

public:
    MENUBUTTON(RTangle BTN, Text INF) : btn(BTN), inf(INF) {}

    virtual ~MENUBUTTON() = default;

    virtual int evaluate(int status) = 0;

    virtual void Draw() {
        btn.Draw();
        inf.Draw();
    }

    void SetButton(const RTangle& newBtn, const Text& newInf) {
        btn = newBtn;
        inf = newInf;
    }
};

class HomeButton : public MENUBUTTON {
public:
    HomeButton(RTangle BTN, Text INF) : MENUBUTTON(BTN, INF) {}
    int evaluate(int status) override {
        int nstatus;
        if (IsButtonClicked(GetMousePosition())) {
            btn.setColor(ORANGE);
            Draw();
            btn.setColor(GREEN);
            nstatus = 0;
            return nstatus;
        }
        Draw();
        return status;
    }
};

class ARRButton : public MENUBUTTON {
public:
    ARRButton(RTangle BTN, Text INF) : MENUBUTTON(BTN, INF) {}
    int evaluate(int status) override {
        int nstatus;
        if (IsButtonClicked(GetMousePosition())) {
            btn.setColor(ORANGE);
            Draw();
            btn.setColor(DARKPURPLE);
            nstatus = 1;
            return nstatus;
        }
        Draw();
        return status;
    }
};

class MATButton : public MENUBUTTON {
public:
    MATButton(RTangle BTN, Text INF) : MENUBUTTON(BTN, INF) {}
    int evaluate(int status) override {
        int nstatus;
        if (IsButtonClicked(GetMousePosition())) {
            btn.setColor(ORANGE);
            Draw();
            btn.setColor(DARKPURPLE);
            nstatus =2;
            return nstatus;
        }
        Draw();
        return status;
    }
};

class BubSortButton : public MENUBUTTON {
public:
    BubSortButton(RTangle BTN, Text INF) : MENUBUTTON(BTN, INF) {}
    int evaluate(int status) override {
        int nstatus = 0;
        if (IsButtonClicked(GetMousePosition())) {
            btn.setColor(ORANGE);
            Draw();
            btn.setColor(DARKPURPLE);
            nstatus = 3;
            return nstatus;
        }
        Draw();
        return status;
    }
};

class   InsSortButton : public MENUBUTTON {
public:
    InsSortButton(RTangle BTN, Text INF) : MENUBUTTON(BTN, INF) {}
    int evaluate(int status) override {
        if (IsButtonClicked(GetMousePosition())) {
            int nstatus;
            btn.setColor(ORANGE);
            Draw();
            btn.setColor(DARKPURPLE);
            nstatus = 4;
            return nstatus;
        }
        Draw();
        return status;
    }
};

class  BubSnew : public MENUBUTTON {
public:
    BubSnew(RTangle BTN, Text INF) : MENUBUTTON(BTN, INF) {}
    int evaluate(int status) override {
        int nstatus = 0;
        if (IsButtonClicked(GetMousePosition())) {
            btn.setColor(ORANGE);
            Draw();
            btn.setColor(DARKPURPLE);
            nstatus = 5;
            return nstatus;
        }
        Draw();
        return status;
    }
};

class  BubSsave : public MENUBUTTON {
public:
    BubSsave(RTangle BTN, Text INF) : MENUBUTTON(BTN, INF) {}
    int evaluate(int status) override {
        int nstatus;
        if (IsButtonClicked(GetMousePosition())) {
            if(BSsave){
            btn.setColor(DARKGREEN);
            Draw();
            btn.setColor(DARKPURPLE);
            nstatus = 6;
            return nstatus;
            }
        }
        else{
        btn.setColor(RED);
        Draw();
        btn.setColor(DARKPURPLE);
        }
        return status;
    }
};

class  InsSnew : public MENUBUTTON {
public:
    InsSnew(RTangle BTN, Text INF) : MENUBUTTON(BTN, INF) {}
    int evaluate(int status) override {
        int nstatus;
        if (IsButtonClicked(GetMousePosition())) {
            btn.setColor(ORANGE);
            Draw();
            btn.setColor(DARKPURPLE);
            nstatus = 7;
            return nstatus;
        }
        Draw();
        return status;
    }
};

class  InsSsave : public MENUBUTTON {
public:
   InsSsave(RTangle BTN, Text INF) : MENUBUTTON(BTN, INF) {}
    int evaluate(int status) override {
        int nstatus;
        if (IsButtonClicked(GetMousePosition())) {
            if(ISsave){
            btn.setColor(DARKGREEN);
            Draw();
            btn.setColor(DARKPURPLE);
            nstatus = 8;
            return nstatus;
            }
        }
        else{
        btn.setColor(RED);
        Draw();
        btn.setColor(DARKPURPLE);
        }
        return status;
    }
};

class  MATnew : public MENUBUTTON {
public:
    MATnew(RTangle BTN, Text INF) : MENUBUTTON(BTN, INF) {}
    int evaluate(int status) override {
        int nstatus;
        if (IsButtonClicked(GetMousePosition())) {
            btn.setColor(ORANGE);
            Draw();
            btn.setColor(DARKPURPLE);
            nstatus = 9;
            return nstatus;
        }
        Draw();
        return status;
    }
};

class  MATSsave : public MENUBUTTON {
public:
   MATSsave(RTangle BTN, Text INF) : MENUBUTTON(BTN, INF) {}
    int evaluate(int status) override {
        int nstatus;
        if (IsButtonClicked(GetMousePosition())) {
            if(MMsave){
            btn.setColor(DARKGREEN);
            Draw();
            btn.setColor(DARKPURPLE);
            nstatus = 10;
            return nstatus;
            }
        }
        else{
        btn.setColor(RED);
        Draw();
        btn.setColor(DARKPURPLE);
        }
        return status;
    }
};

class MAINGUI{
    private:
    PauseButton PS;
    DirrchangeButton CD;
    FPSincButton FI;
    FPSdecButton FD;
    WLKrButton WR;
    WLKlButton WL;
    SELButton SL;
    controls CTRL;
    public:

    MAINGUI(RTangle* defRT, Text* defTXT)
        : PS(defRT[0], defTXT[0]),
          CD(defRT[1], defTXT[1]),
          FI(defRT[2], defTXT[2]),
          FD(defRT[3], defTXT[3]),
          WR(defRT[4], defTXT[4]),
          WL(defRT[5], defTXT[5]),
          SL(defRT[6], defTXT[6]) {}

    controls getConrols(){return CTRL;}
    void setTrck(int k){CTRL.trck=k;}
    void setVall(int k){CTRL.vall=k;}
    void setPause(bool k){CTRL.pause = k;}
    void setWalk(int k){CTRL.walk =k;}

    void EVAL(){
        CTRL = PS.evaluate(CTRL);
        CTRL = CD.evaluate(CTRL);
        CTRL = FI.evaluate(CTRL);
        CTRL = FD.evaluate(CTRL);
        CTRL = WR.evaluate(CTRL);
        CTRL = WL.evaluate(CTRL);
        CTRL = SL.evaluate(CTRL);
    }
};


class BubSortTape{
    private:
    BSListnp BEG;
    BSListnp END;
    int size;

    public:

    BubSortTape(int* VAL, int SIZE ):size(SIZE){
        int temp;
        int finished = false;
        BSListnp curr = new BSNode;
        BEG = new BSNode;
        END = new BSNode;
        int stp = 0;
        BSListnp p = new BSNode;
        int* CVAL = new int[size];
        for(int i = 0;i<size;i++){
            CVAL[i]=VAL[i];
        }

        BEG->val = CVAL;
        BEG->Aelem = -1;
        BEG->Belem = -1;
        BEG->next = curr;
        BEG->prev = NULL;
        BEG->step = 0;
        BEG->ord = YELLOW;
        BEG->fin = false;
        curr = BEG;

        while(!finished){
            finished=true;
            
            for(int i=0;i<SIZE-1;i++){
                stp++;
                CVAL = new int[SIZE];
                for(int i = 0;i<size;i++){
                        CVAL[i]=VAL[i];
                    }
                    p = new BSNode;
                    curr->next = p;
                    p->prev = curr;
                    p->step=stp;
                    p->val = CVAL;
                    p->Aelem=i;
                    p->Belem=i+1;
                    p->fin = finished;
                    p->ord = YELLOW;
                    curr = p;

                if(VAL[i]>VAL[i+1]){
                    temp = VAL[i];
                    VAL[i]=VAL[i+1];
                    VAL[i+1] = temp;
                    finished = false;

                    CVAL = new int[size];
                    for(int i = 0;i<size;i++){
                        CVAL[i]=VAL[i];
                    }

                    p = new BSNode;
                    curr->next = p;
                    p->prev = curr;
                    p->step=stp;
                    p->val = CVAL;
                    p->Aelem=i;
                    p->Belem=i+1;
                    p->fin = finished;
                    p->ord = RED;
                    curr = p;
                }
                else{
                    p = new BSNode;
                    curr->next = p;
                    p->prev = curr;
                    p->step=stp;
                    p->val = CVAL;
                    p->Aelem=i;
                    p->Belem=i+1;
                    p->fin = finished;
                    p->ord = GREEN;
                    curr = p;
                }
            }
        }
        END = curr;
        END->next = NULL;
    }

    void Rend(BSListnp A, int trck, int framerate){
        BSListnp crnt = A;
        char aux[10];
        ClearBackground(RAYWHITE);
        if(A){
        DrawText("|| Is array sorted?(G-YES, R-NO)",310, 700,30, BLACK);
        if(crnt->fin)DrawRectangle(840,698,32,32,GREEN);
        else DrawRectangle(840,698,32,32,RED);
        DrawText("Value ", 20, 500, 30, BLACK);
        DrawText("Index: ", 20, 540, 30, BLACK);
        sprintf(aux, "%d",crnt->step);
        DrawText("Current step:", 20, 700, 30, BLACK);
        DrawText(aux, 250, 700, 30, BLACK );
        sprintf(aux, "%d", framerate);
        DrawText(aux, 1215, 700, 30, BLACK );
        DrawText("|| Framerate(speed):", 900, 700, 30, BLACK );
        DrawRectangle(20,580,32,32,YELLOW);
        DrawRectangle(20,620,32,32,GREEN);
        DrawRectangle(20,660,32,32,RED);
        DrawText("Evaluating elements ", 55, 580, 30, BLACK);
        DrawText("Elements were in order, did nothing", 55, 620, 30, BLACK);
        DrawText("ELements were missplaced, swapped", 55, 660, 30, BLACK);
        DrawText("!!!WHen SEL(selection) button turns yellow, use HOLD A or D to select the element you want to track. Pres SEL again to start tracking it",10,870,23, DARKPURPLE);

        for(int i=0;i<size;i++){
            sprintf(aux, "%d",crnt->val[i]);
            DrawText(aux,205+i*40,500,25,BLACK);
            sprintf(aux, "%d",i);
            DrawText(aux,205+i*40,535,25,BLACK);
            if(i == crnt->Aelem || i== crnt->Belem){
                DrawRectangle(200+i*40,0,35,crnt->val[i]*3,crnt->ord);
            }
            else{if(crnt->val[i] == trck){
                DrawRectangle(200+i*40,0,35,crnt->val[i]*3, PURPLE);
                sprintf(aux, "%d",crnt->val[i]);
            DrawText(aux,205+i*40,500,25,PURPLE);
            sprintf(aux, "%d",i);
            DrawText(aux,205+i*40,535,25,PURPLE);
            }
            else DrawRectangle(200+i*40,0,35,crnt->val[i]*3,BLUE);
            }
        }
        }
    }

    void Display(BSListnp EXAMPLEsave){
        BSListnp crnt = new BSNode;
        int k;

        if(EXAMPLEsave) crnt = EXAMPLEsave;
        else crnt = BEG;

        MAINGUI BS(DEFRT, DEFTXT);
        
        InitWindow(1600,900,"Bubble sort demo");
        SetTargetFPS(60);

        while (!WindowShouldClose()) {
        SetTargetFPS(BS.getConrols().framerate);
        BeginDrawing();

        Rend(crnt, BS.getConrols().vall, BS.getConrols().framerate);
        BSsave = crnt;
        
        BS.EVAL();

        if(BS.getConrols().selection){
        if(IsKeyPressedRepeat(KEY_D))BS.setTrck(k=BS.getConrols().trck+1);
        if(IsKeyPressedRepeat(KEY_A))BS.setTrck(k=BS.getConrols().trck-1);
        BS.setVall(crnt->val[BS.getConrols().trck]);
        }

        if(!BS.getConrols().pause){
            if(BS.getConrols().dir){
                if(crnt->next)crnt=crnt->next;
                else BS.setPause(true);
            }
            else{
                if(crnt->prev)crnt=crnt->prev;
                else BS.setPause(true);

            }
        }
        switch (BS.getConrols().walk){
            case 1:
            if(crnt->next)crnt=crnt->next;
            break;
            case -1:
            if(crnt->prev)crnt=crnt->prev;
            break;
            default:
            break;
        }
        BS.setWalk(0);
        EndDrawing();
        }
        CloseWindow();
    }
};

class InsSortTape{
    private:
    ISListnp BEG;
    ISListnp END;
    int size;
    public:

    InsSortTape(int* VAL, int SIZE):size(SIZE){
        ISListnp curr = new ISNode;
        BEG = new ISNode;
        END = new ISNode;
        int stp = 0;
        int* CVAL = new int[SIZE];
        for(int i = 0;i<SIZE;i++){
            CVAL[i]=VAL[i];
        }
        ISListnp p = new ISNode;

        BEG->val = CVAL;
        BEG->Aelem = -1;
        BEG->keyelem = 0;
        BEG->next = curr;
        BEG->prev = NULL;
        BEG->step = 0;
        BEG->aord = YELLOW;
        curr = BEG;

        for (int i = 1; i < SIZE; ++i) {
            int key =  VAL[i]; 
            int j = i - 1;

             CVAL = new int[SIZE];
             for(int i = 0;i<SIZE;i++){
                    CVAL[i]=VAL[i];
                 }
                    p = new ISNode;
                    curr->next = p;
                    p->prev = curr;
                    p->step=stp;
                    p->val = CVAL;
                    p->Aelem=i;
                    p->Belem=-1;
                    p->keyelem=key;
                    p->aord = BLACK;
                    p->bord= YELLOW;
                    p->Belem = p->Aelem;
                    curr = p;
    
        while (j >= 0 && VAL[j] > key) {
            stp++;
            CVAL = new int[SIZE];
             for(int i = 0;i<SIZE;i++){
                    CVAL[i]=VAL[i];
                 }
                    p = new ISNode;
                    curr->next = p;
                    p->prev = curr;
                    p->step=stp;
                    p->val = CVAL;
                    p->Aelem=j;
                    p->Belem=j+1;
                    p->keyelem=key;
                    p->aord = RED;
                    p->bord = YELLOW;
                    curr = p;
            
            VAL[j + 1] = VAL[j]; 
            --j;

            CVAL = new int[SIZE];
             for(int i = 0;i<SIZE;i++){
                    CVAL[i]=VAL[i];
                 }
                    p = new ISNode;
                    curr->next = p;
                    p->prev = curr;
                    p->step=stp;
                    p->val = CVAL;
                    p->Belem=j+2;
                    p->Aelem=j+1;
                    p->keyelem=key;
                    p->bord = GREEN;
                    p->aord = DARKPURPLE;
                    curr = p;
            }

            VAL[j + 1] = key;
            CVAL = new int[SIZE];
             for(int i = 0;i<SIZE;i++){
                    CVAL[i]=VAL[i];
                 }
                    p = new ISNode;
                    curr->next = p;
                    p->prev = curr;
                    p->step=stp;
                    p->val = CVAL;
                    p->Aelem=j+1;
                    p->Belem = -1;
                    p->keyelem=key;
                    p->aord = DARKGREEN;
                    p->bord = DARKBLUE;
                    curr = p;
        }
    END = curr;
    END->next=NULL;
    }

void Rend(ISListnp A, int trck, int framerate){
        ISListnp crnt = A;
        char aux[10];
        ClearBackground(RAYWHITE);
        if(A){
        DrawText("Value ", 20, 350, 30, BLACK);
        DrawText("Index: ", 20, 390, 30, BLACK);
        sprintf(aux, "%d",crnt->step);
        DrawText("Current step:", 20, 700, 30, BLACK);
        DrawText(aux, 250, 700, 30, BLACK );
        sprintf(aux, "%d", framerate);
        DrawText(aux, 1215, 700, 30, BLACK );
        DrawText("|| Framerate(speed):", 900, 700, 30, BLACK );
        DrawRectangle(20,430,32,32,BLACK);
        DrawRectangle(20,470,32,32,YELLOW);
        DrawRectangle(20,510,32,32,RED);
        DrawRectangle(20,550,32,32,GREEN);
        DrawRectangle(20,590,32,32,DARKPURPLE);
        DrawRectangle(20,630,32,32,DARKGREEN);
        DrawText("Choosen key ", 55, 430, 30, BLACK);
        sprintf(aux,"%d",crnt->keyelem);
        DrawText("Key:",10,10,30,RED);
        DrawRectangle(15,50,40,crnt->keyelem*3,BLACK);
        DrawText(aux, 20,320,30, RED);
        DrawText("Moving element to the right: space for element on the left", 55, 470, 30, BLACK);
        DrawText("Moving element to the right: Element to be moved", 55, 510, 30, BLACK);
        DrawText("Moved element to the right: New place for moved ellement", 55, 550, 30, BLACK);
        DrawText("Moved element to the right: Vacant place to move next element or to insert key", 55, 590, 30, BLACK);
        DrawText(" Key inserted here. Onto the next key....", 55, 630, 30, BLACK);
        DrawText("!!!When SEL(selection) button turns yellow, HOLD keys A or D to select the element you want to track. Pres SEL again to start tracking it",10,870,23, DARKPURPLE);


        
        for(int i=0;i<size;i++){
            sprintf(aux, "%d",crnt->val[i]);
            DrawText(aux,205+i*40,350,25,BLACK);
            sprintf(aux, "%d",i);
            DrawText(aux,205+i*40,385,25,BLACK);
            if(i == crnt->Aelem || i == crnt->Belem ){  
                if(i == crnt->Aelem)DrawRectangle(200+i*40,0,35,crnt->val[i]*3,crnt->aord);
                else DrawRectangle(200+i*40,0,35,crnt->val[i]*3,crnt->bord);
            }
            else{
                if(crnt->val[i] == trck){
                    DrawRectangle(200+i*40,0,35,crnt->val[i]*3, PURPLE);
                    sprintf(aux, "%d",crnt->val[i]);
                    DrawText(aux,205+i*40,350,25,PURPLE);
                    sprintf(aux, "%d",i);
                    DrawText(aux,205+i*40,385,25,PURPLE);
                    }   
            else DrawRectangle(200+i*40,0,35,crnt->val[i]*3,BLUE);
            }
        }
    }
}

void Display(ISListnp EXAMPLEsave){
        ISListnp crnt = new ISNode;
        int k;

        if(EXAMPLEsave) crnt = EXAMPLEsave;
        else crnt = BEG;

        MAINGUI IS(DEFRT, DEFTXT);
        
        InitWindow(1600,900,"Insert sort demo");
        SetTargetFPS(60);

        while (!WindowShouldClose()) {
        SetTargetFPS(IS.getConrols().framerate);
        BeginDrawing();

        Rend(crnt, IS.getConrols().vall, IS.getConrols().framerate);
        ISsave = crnt;
        
        IS.EVAL();

        if(IS.getConrols().selection){
        if(IsKeyPressedRepeat(KEY_D))IS.setTrck(k=IS.getConrols().trck+1);
        if(IsKeyPressedRepeat(KEY_A))IS.setTrck(k=IS.getConrols().trck-1);
        IS.setVall(crnt->val[IS.getConrols().trck]);
        }

        if(!IS.getConrols().pause){
            if(IS.getConrols().dir){
                if(crnt->next)crnt=crnt->next;
                else IS.setPause(true);
            }
            else{
                if(crnt->prev)crnt=crnt->prev;
                else IS.setPause(true);

            }
        }
        switch (IS.getConrols().walk){
            case 1:
            if(crnt->next)crnt=crnt->next;
            break;
            case -1:
            if(crnt->prev)crnt=crnt->prev;
            break;
            default:
            break;
        }
        IS.setWalk(0);
        EndDrawing();
        }
        CloseWindow();
    }
};

class MatMultipTape{
    private:
    MMListnp BEG;
    MMListnp END;
    int size;
    int** MA;
    int** MB;
    public:
    
    MatMultipTape(int** A, int** B, int SIZE) : size(SIZE), MA(A), MB(B) {
    BEG = new MMNode;
    END = new MMNode;
    MMListnp curr = new MMNode;

    int stp = 0;
    int** CVAL = new int*[size];
            for (int x = 0; x < size; x++) {
            CVAL[x] = new int[size];
            }

    int** REZ = new int*[size];
    for (int i = 0; i < size; i++) {
        REZ[i] = new int[size];
        for (int j = 0; j < size; j++) {
            REZ[i][j] = 0;
            CVAL[i][j] = REZ[i][j];
        }
    }

    BEG->I = BEG->J = BEG->K = 0;
    BEG->next = curr;
    BEG->prev = NULL;
    BEG->Rval = CVAL;
    BEG->stp=stp;


    curr = BEG;

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            for (int k = 0; k < size; k++) {

                REZ[i][j] += A[i][k] * B[k][j];

                int** CVAL = new int*[size];
                for (int x = 0; x < size; x++) {
                    CVAL[x] = new int[size];
                    for (int y = 0; y < size; y++) {
                        CVAL[x][y] = REZ[x][y];
                    }
                }

                MMListnp p = new MMNode;
                p->I = i;
                p->J = j;
                p->K = k;
                p->stp = stp++;
                p->Rval = CVAL;
                p->next = NULL;
                p->prev = curr;

                curr->next = p;
                curr = p;
            }
        }
    }

    END = curr;
    END->next = nullptr;

    for (int i = 0; i < size; i++) {
        delete[] REZ[i];
    }
    delete[] REZ;
}

    void Rend(MMListnp THS, int framerate){
        MMListnp crnt = THS;
        char aux[10];
        ClearBackground(RAYWHITE);
        if(THS){
                DrawRectangle(crnt->K*40+20, crnt->I*40+100, 25, 25, PINK);
                DrawRectangle(crnt->J*40+size*50, crnt->K*40+100, 25, 25, BLUE);
                DrawRectangle(crnt->J*80+size*50+400, crnt->I*45+95,65,28, PURPLE);
            for(int i=0;i<size;i++){
                for(int j=0;j<size;j++){
                    sprintf(aux, "%d", MA[i][j]);
                    DrawText(aux, j*40+20, i*40+100, 20, BLACK);
                    sprintf(aux, "%d", MB[i][j]);
                    DrawText(aux, j*40+size*50, i*40+100, 20, BLACK);
                    sprintf(aux, "%d", crnt->Rval[i][j]);
                    DrawText(aux, j*80+size*50+400, i*45+95, 25, BLACK);
                }
            }
            for(int x=0;x<crnt->K+1;x++){
                if(x!=crnt->K) DrawText("+",235+170*x,700,30,BLACK);
                DrawText("(",85+170*x,700,30,BLACK);
                DrawText(")",215+170*x,700,30,BLACK);
                sprintf(aux,"%d",MA[crnt->I][x]);
                DrawText(aux,100+170*x,700,30,BLACK);
                DrawText("*",150+170*x,700,30,BLACK);
                sprintf(aux,"%d",MB[x][crnt->J]);
                DrawText(aux,180+170*x,700,30,BLACK);
                DrawText("=",67,700,30,BLACK);
                DrawRectangle(10,700,50,30,PURPLE);
            }
            sprintf(aux, "%d",crnt->stp);
            DrawText("Current step:", 20, 650, 30, BLACK);
            DrawText(aux, 250, 650, 30, BLACK );
            sprintf(aux, "%d", framerate);
            DrawText(aux, 715, 650, 30, BLACK );
            DrawText("|| Framerate(speed):", 400, 650, 30, BLACK );
            DrawText("*Selection and tracking doesn't work on this demonstration since there's no elements that change their places", 10, 860,25, RED);
        }
    }

    void Display(MMListnp EXAMPLEsave){
        MMListnp crnt = new MMNode;
        if(EXAMPLEsave)crnt=EXAMPLEsave;
        else crnt = BEG;

        MAINGUI MM(DEFRT, DEFTXT);
        
        InitWindow(1600,900,"Matrix multiplication demo");
        SetTargetFPS(60);

        while (!WindowShouldClose()) {
        SetTargetFPS(MM.getConrols().framerate);
        BeginDrawing();

        Rend(crnt, MM.getConrols().framerate);
        MMsave=crnt;

        
        MM.EVAL();

        if(!MM.getConrols().pause){
            if(MM.getConrols().dir){
                if(crnt->next)crnt=crnt->next;
                else MM.setPause(true);
            }
            else{
                if(crnt->prev)crnt=crnt->prev;
                else MM.setPause(true);

            }
        }
        switch (MM.getConrols().walk){
            case 1:
            if(crnt->next)crnt=crnt->next;
            break;
            case -1:
            if(crnt->prev)crnt=crnt->prev;
            break;
            default:
            break;
        }
        MM.setWalk(0);
        EndDrawing();
        }
        CloseWindow();
    }
};

class MAINMENUGUI {
private:
    ARRButton arrButton;
    MATButton matButton;
    BubSortButton bubSortButton;
    InsSortButton insSortButton;
    BubSnew bubSnewButton;
    BubSsave bubSsaveButton;
    InsSnew insSnewButton;
    InsSsave insSsaveButton;
    MATnew matNewButton;
    MATSsave matSaveButton;
    HomeButton home;
    int status=0;

public:
    // RTangle mDEFRT[8]={mARRdefRT, mMATdefRT, mHOMEdefRT, mARRBSdefRT, mARRINSdefRT, mARRNEWdefRT, mSAVEdefRT,mMATNEWdefRT};
    // Text mDEFTXT[8]={mARRdefTXT, mMATdefTXT, mHOMEdefTXT, mARRBSdefTXT, mARRINSdefTXT, mARRNEWdefTXT,mSAVEdefTXT,mMATNEWdefTXT};
    MAINMENUGUI(RTangle* mDEFRT, Text* mDEFTXT)
        : arrButton(mDEFRT[0],mDEFTXT[0]),
          matButton(mDEFRT[1],mDEFTXT[1]),
          bubSortButton(mDEFRT[3],mDEFTXT[3]),
          insSortButton(mDEFRT[4],mDEFTXT[4]),
          bubSnewButton(mDEFRT[5],mDEFTXT[5]),
          bubSsaveButton(mDEFRT[6],mDEFTXT[6]),
          insSnewButton(mDEFRT[5],mDEFTXT[5]),
          insSsaveButton(mDEFRT[6],mDEFTXT[6]),
          matNewButton(mDEFRT[7],mDEFTXT[7]),
          matSaveButton(mDEFRT[6],mDEFTXT[6]),
          home(mDEFRT[2],mDEFTXT[2]) {}

    int getStatus(){return status;}
    void setStatus(int k){status = k;}

    void evaluate(BubSortTape A, InsSortTape B, MatMultipTape C) {
        switch (status){
            case 0:
            setStatus(arrButton.evaluate(status));
            setStatus(matButton.evaluate(status));
            break;

            case 1:
            setStatus(bubSortButton.evaluate(status));
            setStatus(insSortButton.evaluate(status));
            setStatus(home.evaluate(status));
            break;

            case 2:
            setStatus(matNewButton.evaluate(status));
            setStatus(matSaveButton.evaluate(status));
            setStatus(home.evaluate(status));
            break;

            case 3:
            setStatus(bubSnewButton.evaluate(status));
            setStatus(bubSsaveButton.evaluate(status));
            setStatus(home.evaluate(status));
            break;

            case 4:
            setStatus(insSnewButton.evaluate(status));
            setStatus(insSsaveButton.evaluate(status));
            setStatus(home.evaluate(status));
            break;

            case 6:
            CloseWindow();
            A.Display(BSsave);
            setStatus(0);
            InitWindow(600, 600,"MainMenu");
            break;

            case 8:
            CloseWindow();
            B.Display(ISsave);
            setStatus(0);
            InitWindow(600, 600,"MainMenu");
            break;
            
            case 10:
            CloseWindow();
            C.Display(MMsave);
            setStatus(0);
            InitWindow(600, 600,"MainMenu");
            break;

            default:
            break;
        }
    }
};

int sizeinput(bool matrix){
    int max = 30;
    if(matrix)max=9;
    bool sizedone=false;
    char aux[10];
    int size = 1;
    EndDrawing();
    CloseWindow();
    InitWindow(800,250,"making our own array");
    SetTargetFPS(60);

    while(!WindowShouldClose() && !sizedone){
        ClearBackground(RAYWHITE);
        DrawText("Array/Matrix size(maximum 30(9 for matrix),minimum 1):",10, 50, 24,BLACK);
        DrawText("Hold W key to increase size, S key to decrease", 10, 100, 24, DARKGREEN);
        DrawText("Hold ENTER or close window to commit the size", 10, 150, 24, RED);
        sprintf(aux, "%d",size);
        DrawText(aux,680, 38,50,PURPLE);
        if(size<max && IsKeyPressedRepeat(KEY_W))size++;
        else{
            if(size>1 && IsKeyPressedRepeat(KEY_S))size--;
        }
        if(IsKeyPressed(KEY_ENTER))sizedone=true;
        EndDrawing();
    }

    CloseWindow();
    return size;
}

int* arrinput(int size){

int* val=new int[size];
for(int i=0;i<size;i++)val[i]=0;
bool arraydone=false;
int selection =0;
char aux[10];

InitWindow(1600,400,"customizing array");
SetTargetFPS(60);

while(!WindowShouldClose()&&!arraydone){
    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawText("Press R to generate random values and exit, press ENTER or close the window to commit the current array",50, 10,27,RED);
    DrawText("Hold W increase value",50, 60,40,BLACK);
    DrawText("Holds S decrease value",50, 110,40,BLACK);
    DrawText("Press A to move to the element on the left",50, 160,40,BLACK);
    DrawText("Press D to move to the element on the right",50, 210,40,BLACK);
    DrawText("ARRAY:",700, 260,60,DARKGREEN);
    if(IsKeyPressed(KEY_R)){
        for(int i=0;i<size;i++)val[i]=rand()%100;
        arraydone = true;
     }
    else{
    if(IsKeyPressedRepeat(KEY_W)&&val[selection]<100)val[selection]++;
    if(IsKeyPressedRepeat(KEY_S)&&val[selection]>1)val[selection]--;
    if(IsKeyPressed(KEY_D)&&selection<size-1)selection++;
    if(IsKeyPressed(KEY_A)&&selection>0)selection--;
    }
    for(int i =0;i<size;i++){
        sprintf(aux, "%d", val[i]);
        if(i==selection)DrawText(aux, 10+i*52, 350,35,PURPLE);
        else DrawText(aux, 10+i*52, 350,35,BLACK);
    }
    if(IsKeyPressed(KEY_ENTER))arraydone = true;
    EndDrawing();
}
CloseWindow();
return val;
}

int main() {

    int* val = new int[1];
    val[0]=0;

    int** mval = new int*[1];
    for(int i=0;i<1;i++){
        mval[i]=new int[1];
    }
    mval[0][0]=0;

    BubSortTape A(val,1);
    InsSortTape B(val,1);
    MatMultipTape C(mval,mval,1);
    
    MAINMENUGUI MG(mDEFRT, mDEFTXT);
    InitWindow(600, 600,"MainMenu");
    SetTargetFPS(60);
    while(!WindowShouldClose()){
        BeginDrawing();
        ClearBackground(GRAY);
        MG.evaluate(A, B, C);

        if(MG.getStatus()==5){
            int Asize = sizeinput(0);
            int* Aarr = arrinput(Asize);
            BubSortTape AN(Aarr, Asize );
            A = AN;
            A.Display(NULL);
            MG.setStatus(0);
            InitWindow(600, 600,"MainMenu");
            SetTargetFPS(60);
        }

        if(MG.getStatus()==7){
            int Bsize = sizeinput(0);
            int* Barr = arrinput(Bsize);
            InsSortTape BN(Barr, Bsize );
            B = BN;
            B.Display(NULL);
            MG.setStatus(0);
            InitWindow(600, 600,"MainMenu");
            SetTargetFPS(60);
        }

        if(MG.getStatus()==9){
            int Csize = sizeinput(1);
            int** AMat = new int*[Csize];
            for(int i=0;i<Csize;i++){
                AMat[i]=new int[Csize];
                for(int j=0;j<Csize;j++){
                    AMat[i][j]=rand()%100;
                }
            }
            int** BMat = new int*[Csize];
            for(int i=0;i<Csize;i++){
                BMat[i]=new int[Csize];
                for(int j=0;j<Csize;j++){
                    BMat[i][j]=rand()%100;
                }
            }
            MatMultipTape CN(AMat, BMat, Csize );
            C = CN;
            C.Display(NULL);
            MG.setStatus(0);
            InitWindow(600, 600,"MainMenu");
            SetTargetFPS(60);
        }

        EndDrawing();
    }
    return 0;
}