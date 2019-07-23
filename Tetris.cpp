#include<iostream>
#include <random>
using namespace std;
const int board_rows = 32;
const int board_cols = 40;
int play_game = 1;

char** initializeBoard()
{
    char **board;
    int i,j;
    board = new char*[board_rows];
    for(i=0;i<board_rows;i++)
    {
        board[i] = new char[board_cols];
    }
    for(i=0;i<board_cols;i++)
    {
        board[0][i]=' ';
        board[board_rows-1][i]='*';
    }
    for(i=0;i<board_rows;i++)
    {
        board[i][0]='*';
        board[i][board_cols-1]='*';
    }
    for(i=1;i<board_rows-1;i++)
    {
        for(j=1;j<board_cols-1;j++)
        {
            board[i][j]=' ';
        }
    }
    return board;
}
void printboard(char **board)
{
    int i,j;
    cout<<"\n\n";
    for(i=0;i<board_rows;i++)
    {
        for(j=0;j<board_cols;j++)
        {
            cout<<board[i][j];
        }
        cout<<endl;
    }
    cout<<"\n\n";
}
int randomShape()
{
    random_device rd; // obtain a random number from hardware
    mt19937 eng(rd()); // seed the generator
    // mt19937 is A Mersenne Twister pseudo-random generator of 32-bit numbers with a state size of 19937 bits.
    uniform_int_distribution<> distr(0,4); // define the range
    return distr(eng);
}

int randomShapeType()
{
    random_device rd; // obtain a random number from hardware
    mt19937 eng(rd()); // seed the generator
    // mt19937 is A Mersenne Twister pseudo-random generator of 32-bit numbers with a state size of 19937 bits.
    uniform_int_distribution<> distr(0,7); // define the range
    return distr(eng);
}

int randomlocation()
{
    random_device rd; // obtain a random number from hardware
    mt19937 eng(rd()); // seed the generator
    // mt19937 is A Mersenne Twister pseudo-random generator of 32-bit numbers with a state size of 19937 bits.
    uniform_int_distribution<> distr(3,board_cols-5); // define the range
    return distr(eng);
}

class Point
{
    public:
    int x,y;
    public:
    void insert(int a,int b)
    {
        x = a;
        y = b;
    }
    Point()
    {
        x = 0;
        y = 0;
    }
};

class Shape: public Point
{
    protected:
    Point *shape;
    int type;
    public:
    Shape()
    {
        shape = new Point[4];
    }
    ~Shape()
    {
        delete(shape);
    }
    void moveleft(char **board)
    {
        clearshape(board);
        for(int i=0;i<4;i++)
        {
            shape[i].y--;
        }
        if(!verifyboard(board))
        {
            for(int i=0;i<4;i++)
            {
                shape[i].y++;
            }
        }
        putonboard(board);
    }
    void moveright(char **board)
    {
        clearshape(board);clearshape(board);
        for(int i=0;i<4;i++)
        {
            shape[i].y++;
        }
        if(!verifyboard(board))
        {
            for(int i=0;i<4;i++)
            {
                shape[i].y--;
            }
        }
        putonboard(board);
    }
    int movedown(char **board, int *memory)
    {
        clearshape(board);
        for(int i=0;i<4;i++)
        {
            shape[i].x++;
        }
        if(!verifyboard(board))
        {
            cout<<"shape has reached bottom most point so generate a new shape\n";
            moveup(board);
            putonboard(board);
            fixboard(board,memory);
            return 1;
        }
        return 0;
    }
    void moveup(char **board)
    {
        for(int i=0;i<4;i++)
        {
            shape[i].x--;
        }
    }
    void putonboard(char **board)
    {
        for(int i=0;i<4;i++)
        {
            board[shape[i].x][shape[i].y]='#';
        }
    }

    virtual void rotateright(char **board)
    {}

    virtual void rotateleft(char **board)
    {}

    void clearshape(char **board)
    {
        for(int i=0;i<4;i++)
        {
            board[shape[i].x][shape[i].y]=' ';
        } 
    }
    int verifyboard(char **board)
    {
        for(int i=0;i<4;i++)
            {
                if(board[shape[i].x][shape[i].y]!=' ')
                {
                    return 0;
                }
            }
            return 1;
    }
    void fixboard(char **board, int *memory)
    {
        if(memory[0]>0)
        {
            play_game = 0;
            return;
        }
        int i;
        for(i=0;i<4;i++)
        {
            memory[shape[i].x]++;
        }
        for(i=0;i<board_rows;i++)
        {
            if(memory[i]==board_cols-2)
            {
                //this row is full now so clear it
                cout<<"Row "<< i << " is full\n";
                delete [] board[i];
                for(int j=i;j>0;j--)
                {
                    board[j]=board[j-1];
                    memory[j]=memory[j-1];
                }
                board[0] = new char[board_cols];
                board[0][0]=board[0][board_cols-1]='*';
                for(int j=1;j<board_cols-1;j++)
                {
                    board[0][j]=' ';
                }
                memory[0]=0;
            }
        }
    }
    virtual void createShape(int start_x,int start_y,int type)
    {}
};

class Line: public Shape
{
    int type;
    public:
    virtual void createShape(int start_x,int start_y,int type)
    {
        this->type = type;
        if(type==0)
        {
            //create a vertical line -> type = 0
            shape[0].x=start_x+0;shape[0].y=start_y;
            shape[1].x=start_x+1;shape[1].y=start_y;
            shape[2].x=start_x+2;shape[2].y=start_y;
            shape[3].x=start_x+3;shape[3].y=start_y;
        }
        else
        {
            //create a horizontal line -> type = 1
            shape[0].x=start_x;shape[0].y=start_y;
            shape[1].x=start_x;shape[1].y=start_y+1;
            shape[2].x=start_x;shape[2].y=start_y+2;
            shape[3].x=start_x;shape[3].y=start_y+3;
        }
    }
    virtual void rotateright(char **board)
    {
        clearshape(board);
        if(type == 0)
        {
            shape[1].x--; shape[1].y++;
            shape[2].x-=2; shape[2].y+=2;
            shape[3].x-=3; shape[3].y+=3;
            type = 1;
            if(!verifyboard(board))
            {
                type = 0;
                shape[1].x++; shape[1].y--;
                shape[2].x+=2; shape[2].y-=2;
                shape[3].x+=3; shape[3].y-=3;
            }
        }
        else
        {
            type = 0;
            shape[1].x++; shape[1].y--;
            shape[2].x+=2; shape[2].y-=2;
            shape[3].x+=3; shape[3].y-=3;
            if(!verifyboard(board))
            {
                shape[1].x--; shape[1].y++;
                shape[2].x-=2; shape[2].y+=2;
                shape[3].x-=3; shape[3].y+=3;
                type = 1;
            }
        }
        putonboard(board);
    }
    
    virtual void rotateleft(char **board)
    {
        clearshape(board);
        if(type == 0)
        {
            shape[1].x--; shape[1].y++;
            shape[2].x-=2; shape[2].y+=2;
            shape[3].x-=3; shape[3].y+=3;
            type = 1;
            if(!verifyboard(board))
            {
                type = 0;
                shape[1].x++; shape[1].y--;
                shape[2].x+=2; shape[2].y-=2;
                shape[3].x+=3; shape[3].y-=3;
            }
        }
        else
        {
            type = 0;
            shape[1].x++; shape[1].y--;
            shape[2].x+=2; shape[2].y-=2;
            shape[3].x+=3; shape[3].y-=3;
            if(!verifyboard(board))
            {
                shape[1].x--; shape[1].y++;
                shape[2].x-=2; shape[2].y+=2;
                shape[3].x-=3; shape[3].y+=3;
                type = 1;
            }
        }
        putonboard(board);
    }
};

class Square: public Shape
{
    public:
    virtual void createShape(int start_x,int start_y,int type)
    {
        shape[0].x=start_x+0;shape[0].y=start_y;
        shape[1].x=start_x+1;shape[1].y=start_y;
        shape[2].x=start_x+0;shape[2].y=start_y+1;
        shape[3].x=start_x+1;shape[3].y=start_y+1;
    }
    virtual void rotateright(char **board){}
    virtual void rotateleft(char **board){}
};

class Lshape: public Shape
{
    int type;
    public:
    virtual void createShape(int start_x,int start_y,int type)
    {
        this->type = type;
        if(type==0)
        {
            shape[0].x=start_x+0;shape[0].y=start_y;
            shape[1].x=start_x+1;shape[1].y=start_y;
            shape[2].x=start_x+2;shape[2].y=start_y;
            shape[3].x=start_x+2;shape[3].y=start_y+1;
        }
        else if(type==1)
        {
            shape[0].x=start_x;shape[0].y=start_y+2;
            shape[1].x=start_x;shape[1].y=start_y+1;
            shape[2].x=start_x;shape[2].y=start_y;
            shape[3].x=start_x+1;shape[3].y=start_y;
        }
        else if(type==2)
        {
            shape[0].x=start_x+2;shape[0].y=start_y+1;
            shape[1].x=start_x+1;shape[1].y=start_y+1;
            shape[2].x=start_x;shape[2].y=start_y+1;
            shape[3].x=start_x;shape[3].y=start_y;
        }
        else
        {
            shape[0].x=start_x+1;shape[0].y=start_y;
            shape[1].x=start_x+1;shape[1].y=start_y+1;
            shape[2].x=start_x+1;shape[2].y=start_y+2;
            shape[3].x=start_x;shape[3].y=start_y+2;
        }
    }
    virtual void rotateright(char **board)
    {
        clearshape(board);
        if(type==0)
        {
            shape[0].y+=2;
            shape[1].x--;shape[1].y++;
            shape[2].x-=2;
            shape[3].x-=1;shape[3].y-=1;
            type=1;
            if(!verifyboard(board))
            {
                shape[0].y-=2;
                shape[1].x+=1;shape[1].y-=1;
                shape[2].x+=2;
                shape[3].x+=1;shape[3].y+=1;
                type=0;
            }
        }
        else if(type==1)
        {
            shape[0].x+=2;shape[0].y--;
            shape[1].x+=1;
            shape[2].y++;
            shape[3].x--;
            type=2;
            if(!verifyboard(board))
            {
                shape[0].x-=2;shape[0].y++;
                shape[1].x-=1;
                shape[2].y--;
                shape[3].x++;
                type=1;
            }
        }
        else if(type==2)
        {
            shape[0].x--;shape[0].y--;
            shape[2].x++;shape[2].y++;
            shape[3].y+=2;
            type=3;
            if(!verifyboard(board))
            {
                shape[0].x++;shape[0].y++;
                shape[2].x--;shape[2].y--;
                shape[3].y-=2;
                type=2;
            }
        }
        else
        {
            shape[0].x--;
            shape[1].y--;
            shape[2].x++;shape[2].y-=2;
            shape[3].x+=2;shape[3].y--;
            type=0;
            if(!verifyboard(board))
            {
                shape[0].x++;
                shape[1].y++;
                shape[2].x--;shape[2].y+=2;
                shape[3].x-=2;shape[3].y++;
                type=3;
            }
        }
    }

    virtual void rotateleft(char **board)
    {
        clearshape(board);
        if(type==0)
        {
            shape[0].x++;
            shape[1].y++;
            shape[2].x--;shape[2].y+=2;
            shape[3].x-=2;shape[3].y+=1;
            type=3;
            if(!verifyboard(board))
            {
                shape[0].x--;
                shape[1].y--;
                shape[2].x++;shape[2].y-=2;
                shape[3].x+=2;shape[3].y-=1;
                type=0;
            }
        }
        else if(type == 1)
        {
            shape[0].y-=2;
            shape[1].x++;shape[1].y-=1;
            shape[2].x+=2;
            shape[3].x+=1;shape[3].y+=1;
            type=0;
            if(!verifyboard(board))
            {
                shape[0].y+=2;
                shape[1].x-=1;shape[1].y+=1;
                shape[2].x-=2;
                shape[3].x-=1;shape[3].y-=1;
                type=1;
            }
        }
        else if(type == 2)
        {
            shape[0].x-=2;shape[0].y+=1;
            shape[1].x-=1;
            shape[2].y-=1;
            shape[3].x+=1;
            type=1;
            if(!verifyboard(board))
            {
                shape[0].x+=2;shape[0].y-=1;
                shape[1].x+=1;
                shape[2].y+=1;
                shape[3].x-=1;
                type=2;
            }
        }
        else
        {
            shape[0].x+=1;shape[0].y+=1;
            shape[2].x-=1;shape[2].y-=1;
            shape[3].y-=2;
            type=2;
            if(!verifyboard(board))
            {
                shape[0].x-=1;shape[0].y-=1;
                shape[2].x+=1;shape[2].y+=1;
                shape[3].y+=2;
                type=3;
            }
        }
    }
};

class Tshape: public Shape
{
    int type;
    public:
    virtual void createShape(int start_x,int start_y,int type)
    {
        this->type = type;
        if(type==0)
        {
            shape[0].x=start_x;shape[0].y=start_y;
            shape[1].x=start_x;shape[1].y=start_y+1;
            shape[2].x=start_x;shape[2].y=start_y+2;
            shape[3].x=start_x+1;shape[3].y=start_y+1;
        }
        else if(type==1)
        {
            shape[0].x=start_x;shape[0].y=start_y+1;
            shape[1].x=start_x+1;shape[1].y=start_y+1;
            shape[2].x=start_x+2;shape[2].y=start_y+1;
            shape[3].x=start_x+1;shape[3].y=start_y;
        }
        else if(type==2)
        {
            shape[0].x=start_x+1;shape[0].y=start_y+2;
            shape[1].x=start_x+1;shape[1].y=start_y+1;
            shape[2].x=start_x+1;shape[2].y=start_y;
            shape[3].x=start_x;shape[3].y=start_y+1;
        }
        else
        {
            shape[0].x=start_x+2;shape[0].y=start_y;
            shape[1].x=start_x+1;shape[1].y=start_y;
            shape[2].x=start_x+0;shape[2].y=start_y;
            shape[3].x=start_x+1;shape[3].y=start_y+1;
        }
    }
    virtual void rotateright(char **board)
    {

    }
    virtual void rotateleft(char **board)
    {

    }
};

class Sshape: public Shape
{
    int type;
    public:
    virtual void createShape(int start_x,int start_y,int type)
    {
        this->type = type;
        if(type==0)
        {
            //create a vertical S -> type = 0
            shape[0].x=start_x+0;shape[0].y=start_y;
            shape[1].x=start_x+1;shape[1].y=start_y;
            shape[2].x=start_x+1;shape[2].y=start_y+1;
            shape[3].x=start_x+2;shape[3].y=start_y+1;
        }
        else if(type==1)
        {
            //create a horizontal S -> type = 1
            shape[0].x=start_x;shape[0].y=start_y+2;
            shape[1].x=start_x;shape[1].y=start_y+1;
            shape[2].x=start_x+1;shape[2].y=start_y+1;
            shape[3].x=start_x+1;shape[3].y=start_y;
        }
    }
    virtual void rotateright(char **board)
    {

    }
    virtual void rotateleft(char **board)
    {
        
    }
};

int main()
{
    char **board = initializeBoard();
    int *memory = new int[board_rows-1];
    for(int i=0;i<board_rows-1;i++)
    {
        memory[i]=0;
    }
    printboard(board);
    Shape *s;
    int oldobj = 0;
    int shape_type;
    char userinput;
    while(play_game)
    {
        int current_coordinate_x=0,current_coordinate_y=0;
        if(!oldobj)
        {
            oldobj = 1;
            current_coordinate_y = randomlocation();
            int sh = randomShape();
            shape_type = randomShapeType();
            if(sh == 0)
            {
                cout<<"Line\n";
                s = new Line();
                s->createShape(current_coordinate_x,current_coordinate_y,shape_type%2);
            }
            else if(sh==1)
            {
                cout<<"Lshape\n";
                s = new Lshape();
                s->createShape(current_coordinate_x,current_coordinate_y,shape_type%4);
            }
            else if(sh == 2)
            {
                cout<<"Sshape\n";
                s = new Sshape();
                s->createShape(current_coordinate_x,current_coordinate_y,shape_type%2);
            }
            else if(sh == 3)
            {
                cout<<"Tshape\n";
                s = new Tshape();
                s->createShape(current_coordinate_x,current_coordinate_y,shape_type%4);
            }
            else
            {
                cout<<"square\n";
                s = new Square();
                s->createShape(current_coordinate_x,current_coordinate_y,shape_type);
            }
            s->putonboard(board);
            printboard(board);
        }
        cin>>userinput;
        if(userinput == 'l')
        {
            s->moveleft(board);
        }
        else if(userinput == 'r')
        {
            s->moveright(board);
        }
        else if(userinput == 'e')
        {
            break;
        }
        else if(userinput == 'a')
        {
            s->rotateleft(board);
        }
        else if(userinput == 'c')
        {
            s->rotateright(board);
        }
        else if(userinput == 'z')
        {
            //undo changes
        }
        else if(userinput == 'x')
        {
            //redo changes
        }
        if(s->movedown(board,memory))
        {
            oldobj = 0;
            continue;
        }
        s->putonboard(board);
        printboard(board);
    }
    for(int i = 0; i < board_rows; i++)
	{
		delete [] board[i];
	}
	delete [] board;
    delete(s);
}

