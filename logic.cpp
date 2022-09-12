#include <iostream>
#include <fstream>
#include <string>
#include "logic.h"

using namespace std;
char** loadLevel(const string& fileName, int& maxRow, int& maxCol, Player& player) {
    ifstream fin(fileName);
    
    int numLines = 0;
    bool hasExit = false;
    bool hasDoor = false;
    
    if(!fin.is_open())
        return nullptr;
    maxRow = INT32_MAX;
    maxCol = INT32_MAX;
    player.row = INT32_MAX;
    player.col = INT32_MAX;
    

    fin >> maxRow >> maxCol;
    fin >> player.row >> player.col;
   
    if(maxCol >= INT32_MAX || maxRow >= INT32_MAX || player.row>= INT32_MAX || player.col >= INT32_MAX)
        return nullptr;
    if(INT32_MAX/maxCol <= maxRow)
        return nullptr; 
    if(maxRow == 1 && maxCol == 1)
        return nullptr;
    if(maxRow <= 0 || maxCol <= 0)
        return nullptr;
    if(player.row >= maxRow || player.col >= maxCol)
        return nullptr;
    if(player.row < 0 && player.col < 0)
        return nullptr;

    char **map = new char *[maxRow];



    for(int i = 0; i < maxRow; i++){
        map[i] = new char[maxCol];
    }
    for(int i =0; i<maxRow; i++){
    
        for(int j = 0; j<maxCol; j++){
            fin >> map[i][j];
            if(map[i][j] != TILE_TREASURE && map[i][j] != TILE_AMULET && map[i][j] != TILE_DOOR && map[i][j] != TILE_EXIT && map[i][j] != 
            TILE_OPEN && map[i][j] != TILE_MONSTER && map[i][j] != TILE_PILLAR && map[i][j] != TILE_PLAYER)
                return nullptr;
            if(map[i][j] == TILE_PLAYER)
                map[i][j] = TILE_OPEN;
            if(map[i][j] == TILE_EXIT)
                hasExit = true;
            if(map[i][j] == TILE_DOOR)
                hasDoor = true;
         //   cout << map[i][j];
            
        }
     //   cout << endl;
    }
    if(map[player.row][player.col] != TILE_OPEN)
        return nullptr;
    else
        map[player.row][player.col] = 'o';
    if(!hasExit && !hasDoor)
        return nullptr;
    if(hasExit && hasDoor)
        return nullptr;
    
    string line = "";
    ifstream sameFile(fileName);
    while(getline(sameFile,line)){
  //      cout << line << endl;
        numLines++;
    }
    if(numLines-2 != maxRow)
        return nullptr;
    string line2 = "";
    int counter = 0;
    int count = 0;
    ifstream sameFileAgain(fileName);
    while(getline(sameFileAgain,line2)){
        if(counter >= 2)
        {
        //    cout << line2.length();
            if(line2.length()-maxCol != maxCol-1)
                return nullptr;
        }       
        counter++;
       
    }
    return map;
}


void getDirection(char input, int& nextRow, int& nextCol) {
    if(input == MOVE_UP)
        nextRow -= 1;
    if(input == MOVE_DOWN)
        nextRow += 1;
    if(input == MOVE_RIGHT)
        nextCol += 1;
    if(input == MOVE_LEFT)
        nextCol -= 1;

}

char** createMap(int maxRow, int maxCol) {
    return nullptr;
}

void deleteMap(char**& map, int& maxRow) {

    if(map!= nullptr){
        for(int i = 0; i <maxRow; i++)
            delete[] map[i];
        delete[] map;
        map = nullptr;
        maxRow = 0;
    }
    else{
        maxRow = 0;
    }
    
}

char** resizeMap(char** map, int& maxRow, int& maxCol) {

    int newMaxRow = maxRow * 2;
    int newMaxCol = maxCol * 2;

    if(map == nullptr)
        return nullptr;
    if(maxRow < 1 || maxCol < 1)
        return nullptr;
 

  
    char **newMap = new char *[newMaxRow];   

    for(int i = 0; i < newMaxRow; i++){
        newMap[i] = new char[newMaxCol];
    }
    for(int i = 0; i<newMaxRow; i++){
        for(int j = 0; j<newMaxCol; j++){
            newMap[i][j] = TILE_OPEN;    
        }
    }
    for(int i = 0; i<maxRow; i++)
    {
        for(int j = 0; j <maxCol; j++)
        {
            newMap[i][j] = map[i][j];
            
        }
        
    }
    int counter = 0;

    for(int i = maxRow; i<newMaxRow; i++)
    {
        for(int j = 0; j<maxCol; j++)
        {
            if(map[counter][j] == TILE_PLAYER)
                newMap[i][j] = TILE_OPEN;
            else
                 newMap[i][j] = map[counter][j];
        }
        counter++;
    }
    counter = 0;
    for(int i = 0; i<maxRow; i++)
    {
        for(int j = maxCol; j<newMaxCol; j++)
        {
            if(map[i][counter] == TILE_PLAYER)
                newMap[i][j] = TILE_OPEN;
            else
                newMap[i][j] = map[i][counter];
            counter++;
        }
        
        counter = 0;
    }
    counter = 0;
    int secondCounter = 0;
    for(int i = maxRow; i<newMaxRow; i++)
    {
        for(int j = maxCol; j<newMaxCol; j++)
        {
            if(map[counter][secondCounter] == TILE_PLAYER)
                newMap[i][j] = TILE_OPEN;
            else
                newMap[i][j] = map[counter][secondCounter];
            secondCounter++;
        }
        secondCounter = 0;
        counter++;
    }

    
    int tempRow = maxRow;
    deleteMap(map,tempRow);
    maxRow *= 2;
    maxCol *= 2;
    return newMap; 
}

int doPlayerMove(char** map, int maxRow, int maxCol, Player& player, int nextRow, int nextCol) {


    if(nextRow >= maxRow || nextCol >= maxCol || nextRow < 0 || nextCol < 0){
        nextRow = player.row;
        nextCol = player.col;
        return STATUS_STAY;
              
    }
         
    if(map[nextRow][nextCol] == TILE_PILLAR || map[nextRow][nextCol] == TILE_MONSTER){
        nextRow = player.row;
        nextCol = player.col;
        return STATUS_STAY;
    }
    if(map[nextRow][nextCol] == TILE_TREASURE){
        player.treasure += 1;
        map[player.row][player.col] = TILE_OPEN;
        player.row = nextRow;
        player.col = nextCol;
        map[player.row][player.col] = TILE_PLAYER;
        return STATUS_TREASURE;
    }
    if(map[nextRow][nextCol] == TILE_DOOR){
        map[player.row][player.col] = TILE_OPEN;
        player.row = nextRow;
        player.col = nextCol;
        map[player.row][player.col] = TILE_PLAYER;
        return STATUS_LEAVE;    
    }
    if(map[nextRow][nextCol] == TILE_AMULET)
    {
        map[player.row][player.col] = TILE_OPEN;
        player.row = nextRow;
        player.col = nextCol;
        map[player.row][player.col] = TILE_PLAYER;
        return STATUS_AMULET;
    }
    
    if(map[nextRow][nextCol] == TILE_EXIT){
        if(player.treasure >= 1){
            map[player.row][player.col] = TILE_OPEN;
            player.row = nextRow;
 //           cout << player.row;
            player.col = nextCol;
 //           cout << player.col;
            map[player.row][player.col] = TILE_PLAYER;   
            return STATUS_ESCAPE;
        }
        else{
            nextRow = player.row;
            nextCol = player.col;
            return STATUS_STAY;
        }
    }
    if(map[nextRow][nextCol] == TILE_OPEN){
        map[player.row][player.col] = TILE_OPEN;
        player.row = nextRow;
        player.col = nextCol;
        map[player.row][player.col] = TILE_PLAYER;
        return STATUS_MOVE;
    }
    return STATUS_STAY;
}

bool doMonsterAttack(char** map, int maxRow, int maxCol, const Player& player) {

    
      
    for(int i = player.row-1; i>=0; i--){ 
        if(map[i][player.col]==TILE_PILLAR) //Pillar before monster
            break;
        else if(map[i][player.col] == TILE_MONSTER){
            map[i+1][player.col]= TILE_MONSTER;
            map[i][player.col] = TILE_OPEN;
            
        }
    }
   
    for(int i = player.row+1; i< maxRow; i++){
        if(map[i][player.col] == TILE_PILLAR)
            break;
        else if(map[i][player.col] == TILE_MONSTER){
            map[i-1][player.col] = TILE_MONSTER;
            map[i][player.col] = TILE_OPEN;
        }
    }

    
    for(int i = player.col-1; i>=0; i--){
        if(map[player.row][i] == TILE_PILLAR)
            break;
        else if(map[player.row][i] == TILE_MONSTER){
            map[player.row][i+1] = TILE_MONSTER;
            map[player.row][i] = TILE_OPEN;
        }
    }
    
    for(int i = player.col+1; i <maxCol; i++){
        if(map[player.row][i] == TILE_PILLAR)
            break;
        else if(map[player.row][i] == TILE_MONSTER){
            map[player.row][i-1]=TILE_MONSTER;
            map[player.row][i] = TILE_OPEN;
        }
    }

    if(map[player.row][player.col] == TILE_MONSTER)
        return true;
    else   
        return false;
    
    
}
