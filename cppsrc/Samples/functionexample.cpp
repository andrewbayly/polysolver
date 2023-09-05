#include "functionexample.h"

#include <iostream>
#include <vector>
#include <string>
#include <sstream>

class Rectangle { 
public: 
  Rectangle(int _x, int _y){ 
    x = _x; 
    y = _y; 
  }
  int x; 
  int y; 
};

class Polyomino {
public: 
  Polyomino(std::vector<std::vector<int>> _vertices){ 
    vertices = _vertices;
    maxX = 0; 
    maxY = 0; 
    
    setMax(); 
  } 
  
  std::vector<std::vector<int>> vertices;
  int maxX; 
  int maxY; 
  
  void setMax(){ 
    for(size_t i = 0; i < vertices.size(); i++){ 
      int x = vertices[i][0]; 
      int y = vertices[i][1]; 
      if(x > maxX)
        maxX = x;
      if(y > maxY)
        maxY = y; 
    }   
  }
  
  Polyomino* clone(){ 
    Polyomino* r = new Polyomino({});
    r->vertices = vertices; 
    r->setMax(); 
    return r;  
  }
  
  void debug(){ 
    std::cout << vertices[0][0] 
              << vertices[0][1] << std::endl ;
              //<< vertices[1][0] 
              //<< vertices[1][1]  
  } 
  
  Polyomino* rotate(){
    for(size_t i = 0; i < vertices.size(); i++){ 
      int x = vertices[i][0]; 
      int y = vertices[i][1]; 
      
      vertices[i][0] = maxY - y; 
      vertices[i][1] = x; 
    }
  
    int _maxX = maxX ; 
    int _maxY = maxY ;
    
    maxX = _maxY; 
    maxY = _maxX; 
    
    return this; 
   
  }
  
  Polyomino* reflect(){ 
    for(size_t i = 0; i < vertices.size(); i++){ 
      int x = vertices[i][0]; 
      int y = vertices[i][1]; 
      
      vertices[i][0] = maxX - x; 
      vertices[i][1] = y; 
    }
    
    return this; 
  }  
  
}; 

class Constraint;

class Column; 

class Link {
public:
    Link(){ 
      up = NULL; 
      down = NULL; 
      left = NULL; 
      right = NULL; 
      column = NULL; 
    }
    Link* up;
    Link* down;
    Link* right;
    Link* left;
    Column* column;
};

class Column : public Link {
public:
    Column(){
    }
    Column(Constraint* _constraint){ 
      constraint = _constraint; 
    }
    Constraint* constraint;
    int size;
};

class Constraint {
public:
    Constraint(int _index, int _x, int _y){ 
      index = _index; 
      x = _x; 
      y = _y; 
      count = 0; 
      placement = -1; 
    }
    int index;
    int x;
    int y;
    std::vector<int> placements;
    int count;
    int placement;
};

class Placement {
public:
    Placement(int _index){ 
      index = _index; 
    }
    int index;
    std::vector<int> constraints;
    int count;

    void add(Constraint* constraint) {
//    std::cout << "A;";

        constraints.push_back(constraint->index);
//    std::cout << "B;";

//    std::cout << constraint->index << ";"; 

//    std::cout << "D;";
        constraint->placements.push_back(index);
//    std::cout << "C;";

    }
};

class Head : public Column {
public:
    Head(){
    }
};


class DLX {
private:
    std::vector<std::vector<Link*>> matrix;
    Head* head;
    std::vector<Link*> solution;

    void cover(Column* col) {
    
//std::cout << "31;";    
        col->right->left = col->left;
        col->left->right = col->right;

        Link* i = col->down;
        
//std::cout << "[" << col->constraint->x << "|" << col->constraint->y << "]"       

/**
std::cout << col << ";" ;        
std::cout << i << ";";        
std::cout << i->down << ";";        
std::cout << i->down->down << ";" ;        
std::cout << i->down->down->down << ";" ;        
std::cout << i->down->down->down->down << ";" ;        
**/

        
        while (i != (Link*)col) {
//std::cout << "32;";
            Link* j = i->right;
            while (j != i) {
//std::cout << "33;";
                j->down->up = j->up;
                j->up->down = j->down;

                j->column->size--;

                j = j->right;
            }
            i = i->down;
        }
//std::cout << "34;";        
    }

    void uncover(Column* col) {
        Link* i = col->up;
        while (i != (Link*)col) {
            Link* j = i->left;
            while (j != i) {
                j->down->up = j;
                j->up->down = j;

                j->column->size++;

                j = j->left;
            }
            i = i->up;
        }

        col->right->left = col;
        col->left->right = col;
    }

    Column* chooseColumn() {
        Column* c = (Column*)(head->right);
        int size = c->size;
        Column* chosen = c;

        while (c != head) {
            if (c->size < size) {
                chosen = c;
                size = c->size;
            }
            c = (Column*)(c->right);
        }
        return chosen;
    }

    bool search(int k) {
    
//  std::cout << k << ";26;";
  
        if (head->right == head) {
  //std::cout << "solution found;";
        
            printSolution();
            return true;
        } else {
            Column* c = chooseColumn();

//  std::cout << "27;";

            cover(c);
            Link* r = c->down;
            while (r != c) {
//  std::cout << "28;";

                solution.push_back(r);
                Link* j = r->right;

                while (j != r) {
                    cover(j->column);
                    j = j->right;
                }

                bool result = search(k + 1);

                if(result){ 
                  return true; 
                }

                solution.pop_back(); 
                
                j = r->left;
                while (j != r) {
                    uncover(j->column);
                    j = j->left;
                }
                r = r->down;
            }
//  std::cout << "29;";          
            uncover(c);
//  std::cout << "30;";
        }
        
        return false; 
    }

public:
    std::vector<Placement*> placements;
    std::vector<Constraint*> constraints;
    std::vector<std::vector<char>> display;
    int untiled;
    std::string result;
    std::vector<std::vector<Constraint*>> grid; 

    void initDisplay() {
//        std::cout << "13;";

        for (size_t j = 0; j < grid[0].size(); j++) {
            std::vector<char> row;
            for (size_t i = 0; i < grid.size(); i++) {
                row.push_back('.');
            }
            display.push_back(row);
        }
//        std::cout << "14;";

    }

    void initialize(Rectangle* rect, Polyomino* poly) {
        //std::vector<Placement*> placements;
        //std::vector<Constraint*> constraints;

        grid.clear(); 
        
//        std::cout << "10;";

        constraints.clear(); 

        //std::cout << rect->x << ";";
        //std::cout << rect->y << ";";


        int index = 0;
        for (int i = 0; i < rect->x; i++) {
            std::vector<Constraint*> column;
            for (int j = 0; j < rect->y; j++) {
                Constraint* c = new Constraint(index, i, j);
                column.push_back(c);
                constraints.push_back(c);
                index++;
            }
            grid.push_back(column);
        }
        
//        std::cout << grid.size() << ";"; 
//        std::cout << grid[0].size() << ";"; 

//        std::cout << "11;";

        initDisplay();

        std::vector<Polyomino*> polyominos;
        polyominos.push_back(poly);


        for (size_t i = 0; i < 7; i++) {
            polyominos.push_back(poly->clone());
        }

        // Rotate and reflect polyomino to generate different variations
        // ... (implementation omitted for brevity)

//        std::cout << "12;";

        polyominos[1]->rotate();
        polyominos[2]->rotate()->rotate();
        polyominos[3]->rotate()->rotate()->rotate();
  
        polyominos[4]->reflect();
        polyominos[5]->reflect()->rotate();
        polyominos[6]->reflect()->rotate()->rotate();
        polyominos[7]->reflect()->rotate()->rotate()->rotate();
 
        
/*
        polyominos[0]->debug();

        polyominos[1]->debug();
        polyominos[2]->debug();
        polyominos[3]->debug();

        polyominos[4]->debug();
        polyominos[5]->debug();
        polyominos[6]->debug();
        polyominos[7]->debug();
*/


//        std::cout << "15;";
        
        placements.clear();

//        std::cout << "16;";
 
        int placementIndex = 0;

        for (int i = 0; i < rect->x; i++) {
            for (int j = 0; j < rect->y; j++) {
                for (size_t p = 0; p < polyominos.size(); p++) {
                    // ... (implementation omitted for brevity)
                    if(i + polyominos[p]->maxX < rect->x && j + polyominos[p]->maxY < rect->y){ 
                    
//        std::cout << "18;";
 
                        Placement* placement = new Placement(placementIndex);
                        placements.push_back(placement);
        
//        std::cout << "19;\n";
   
                        for(size_t c = 0; c < polyominos[p]->vertices.size(); c++){ 
                        
//        std::cout << "21;";
        
//        std::cout << "[" << i + polyominos[p]->vertices[c][0] << "|" << j + polyominos[p]->vertices[c][1] << "];" ;
        
        
                            placement->add(grid[i + polyominos[p]->vertices[c][0]][j + polyominos[p]->vertices[c][1]]);           
//        std::cout << "22;";

                        }
         
//        std::cout << "20;";
  
         
                        placementIndex++; 
                    }     
                }
            }
        }
        
//        std::cout << "17;";


        untiled = constraints.size();
        init();
    }

    void init() {
        // Create the matrix from the given constraints and placements

//std::cout << "25;";
        std::vector<std::vector<Link*>> matrix(this->constraints.size(), std::vector<Link*>(this->placements.size(), nullptr));

        for (size_t p = 0; p < this->placements.size(); ++p) {
            for (size_t c = 0; c < this->placements[p]->constraints.size(); ++c) {
                matrix[this->placements[p]->constraints[c]][p] = new Link();
            }
        }

//std::cout << "26;";

        this->head = new Head();
        Column* last = this->head;

        for (size_t i = 0; i < this->constraints.size(); ++i) {
            Column* col = new Column(this->constraints[i]);
            col->left = last;
            last->right = col;
            last = col;
        }

//std::cout << "27;";

        last->right = this->head;
        this->head->left = last;

        // Create the vertical links
        Column* col = this->head;

        for (size_t i = 0; i < matrix.size(); ++i) {
            col = (Column*)(col->right);

            Link* lastLink = (Link*)col;
            size_t size = 0;

            for (size_t j = 0; j < matrix[i].size(); ++j) {
                if (matrix[i][j] != nullptr) {
                    Link* x = matrix[i][j];    
                    lastLink->down = x;
                    x->up = lastLink;
                    x->column = col;
                    
                    lastLink = x; 
                    //if (col->down != nullptr) {
                    //    col->down->up = lastLink;
                    //    lastLink->down = col->down;
                    // }

                    ++size;
                }
            }

            col->size = size;

            lastLink->down = col;
            col->up = lastLink; 
            
        }
//std::cout << "28;";
        

        // Create the horizontal links
        for (size_t row = 0; row < matrix[0].size(); ++row) {
            Link* temp = new Link();
            Link* lastLink = temp;

            for (size_t col = 0; col < matrix.size(); ++col) {
                if (matrix[col][row] != nullptr) {
                    matrix[col][row]->left = lastLink;
                    lastLink->right = matrix[col][row];
                    lastLink = lastLink->right;
                }
            }

            temp->right->left = lastLink;
            lastLink->right = temp->right;
        }
        
//std::cout << "29;";
        

        this->solution.clear();
        this->result = "[]" ; 
    }

    void printSolution() {

        // ... (implementation omitted for brevity)
        std::vector<std::vector<int>> _solution; 
        
        for(size_t j = 0 ; j < grid[0].size(); j++){ 
            std::vector<int> row;  
            for(size_t i = 0; i < grid.size(); i++){ 
                row.push_back(-1);
            }
            _solution.push_back(row);
        }

        int color = 0; 
  
        for(size_t i = 0; i < solution.size(); i++){ 
            std::vector<Link*> row; 
            Link* rowStart = solution[i]; 
            row.push_back(rowStart); 
            Link* link = rowStart->right; 
            while(link != rowStart){ 
                row.push_back(link);
                link = link->right;
            }
    
            std::vector<Constraint*> constr; 

            //var constr = row.map(x => x.column.constraint)
            for(size_t i = 0; i < row.size(); i++){ 
              constr.push_back(row[i]->column->constraint); 
            }
            
            //const color = String.fromCharCode(65 + Math.floor(Math.random() * 26))  
            for(size_t i = 0; i < constr.size(); i++){ 
              _solution[constr[i]->y][constr[i]->x] = color; 
            }
            
            color++; 
        }
        
        this->result = "[" ; 
        
        //this.result = _solution.map(x => x.join('')).join('\n')
        for(size_t r = 0; r < _solution.size(); r++){ 
          if(r > 0){
            this->result += ",\n"; 
          }
          this->result += "["; 
          for(size_t c = 0; c < _solution[r].size(); c++){ 
            if(c > 0){
              this->result += ","; 
            }
            this->result += std::to_string(_solution[r][c]);  
          }
          this->result += "]"; 
        } 
        this->result += "]" ; 

   
    }

    void solve() {
        // ... (implementation omitted for brevity)
//        std::cout << "2;"; 

        this->search(0); 

//        std::cout << "3;"; 

    }
};

std::string functionexample::solve(std::string x){
  
  DLX dlx; 

  std::string str = x.substr(1, x.size() - 2 ); //remove leading and trailing []
  
  //std::cout << str << "\n" ; 
  
  std::stringstream ss( str );
  std::vector<int> vect;

  while( ss.good() )
  {
    std::string substr;
    getline( ss, substr, ',' );
    vect.push_back( atoi(substr.c_str()) );
  }

/**
  std::stringstream<char> ss(str);

  for (int i; ss >> i;) {
    vect.push_back(i);    
    if (ss.peek() == ',' || ss.peek() == ' ')
      ss.ignore();
  }

  for (std::size_t i = 0; i < vect.size(); i++)
    std::cout << vect[i] << std::endl;
**/  
  
  int _x = vect[0];
  int _y = vect[1];
  
  Rectangle* rect = new Rectangle(_x, _y);
  
  std::vector<std::vector<int>> points; 
  
  for(size_t i = 2; i < vect.size(); i += 2){
    std::vector<int> point;
    point.push_back(vect[i]); 
    point.push_back(vect[i+1]); 
    
    points.push_back(point);      
  }
  
  //dlx.initialize(rect, new Polyomino(poly))

/**  
  std::vector<int> p1 {0, 0}; 
  std::vector<int> p2 {0, 1}; 
  std::vector<int> p3 {0, 2}; 
  std::vector<int> p4 {0, 3}; 
  std::vector<int> p5 {0, 4}; 
  std::vector<int> p6 {1, 1}; 
  
  std::vector<std::vector<int>> points(6); 
  points[0] = p1;
  points[1] = p2; 
  points[2] = p3; 
  points[3] = p4; 
  points[4] = p5; 
  points[5] = p6; 
**/  
  
  dlx.initialize(rect, new Polyomino(points)); 

  dlx.solve();

  return dlx.result;
}

Napi::String functionexample::SolveWrapped(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    Napi::String first = info[0].As<Napi::String>();
    Napi::String returnValue = Napi::String::New(env, functionexample::solve(first));

    return returnValue;
}

Napi::Object functionexample::Init(Napi::Env env, Napi::Object exports)
{
  //exports.Set("hello", Napi::Function::New(env, functionexample::HelloWrapped));
  //exports.Set("add", Napi::Function::New(env, functionexample::AddWrapped));
  exports.Set("solve", Napi::Function::New(env, functionexample::SolveWrapped));

  return exports;
}
