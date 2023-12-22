#include <iostream>
#include <sstream>
#include <string>

using namespace std;

#include "globals.h"
#include "Shape.h"

// This is the shape array, to be dynamically allocated
Shape** shapesArray;

// The number of shapes in the database, to be incremented 
int shapeCount = 0;

// The value of the argument to the maxShapes command
int max_shapes;

void max_shape_in(int num_shapes);
void error_cases(int error_num, string name);
void trailing_white_space(stringstream& ss);

//helper funcs used for create
bool check_error_create(bool endline, bool correct_type, string name, string type, int size_location_arg);
bool check_shape_name(string name);
bool check_shape_type(string type);
bool check_valid_name(string name);
void delete_data_base();

//helpers for move
bool loc_in_bounds(int location);
bool check_error_move(bool endline, bool correct_type, int location, string name);
void execute_move(string name, int loc1, int loc2);

//helpers for rotate
bool rotate_in_bounds(int angle);
bool check_error_rotate(bool endline, bool correct_type, int angle, string name);
void execute_rotate(string name, int angle);

//helpers for draw and delete
bool check_error_draw(bool correct_type, string input_arg);
void draw_all();
void draw_selected(string name);
void delete_all(bool output_deleted); //input if you want it to output "deleted all shapes"
void delete_selected(string name);


int main() {
    bool database_created = false;
    string line;
    string command;
    
    cout << "> ";         // Prompt for input
    getline(cin, line);    // Get a line from standard input

    while ( !cin.eof () ) {
        // Put the line in a linestream for parsing
        // Making a new sstream for each line so flags etc. are cleared
        stringstream lineStream (line);
        
        // Read from string stream into the command
        // The only way this can fail is if the eof is encountered
        lineStream >> command;

        
        if(lineStream.fail()){
            lineStream.clear();
            error_cases(0, "NULL");
        }


        else if(command == "maxShapes"){ //LOGIC FOR maxShapes Command
            bool error = false;
            if(lineStream.eof()){ //it is already the end of the lineStream and that means there is no value
                error_cases(8, "NULL"); // too few arguemnts
                error = true;
            }
            if(!error && database_created){
                delete_all(false);
                lineStream >> max_shapes;
                if(lineStream.fail()){
                    error_cases(1, "NULL");
                }
                else if(!lineStream.eof()){ //Taken in everything but still not end, too many args
                    error_cases(7, "NULL");
                }
                else{
                    max_shape_in(max_shapes);
                }
            }
            if(!error && !database_created){
                database_created = true;
                lineStream >> max_shapes;
                if(lineStream.fail()){
                    error_cases(1, "NULL");
                }
                else if(!lineStream.eof()){ //Taken in everything but still not end, too many args
                    error_cases(7, "NULL");
                }
                else{
                    max_shape_in(max_shapes);
                }
            }
        }


        else if(command == "create"){ //LOGIC for create command
            bool error = false;
            string name;
            string type;
            int x_location = 0;
            int y_location = 0;
            int x_size = 0;
            int y_size = 0;
            if(lineStream.eof()){
                error_cases(8, "NULL"); //too few arguments
                error = true;
            }
            if(!error){
                //name
                lineStream >> name;
                trailing_white_space(lineStream);
                if(check_error_create(lineStream.eof(), lineStream.fail(), name, "circle", x_location)){ // also check for correct name
                    //type
                    lineStream >> type;
                    trailing_white_space(lineStream);
                    if(check_error_create(lineStream.eof(), lineStream.fail(), name, type, x_location)){ //also check for correct type
                        //x_location
                        lineStream >> x_location;
                        trailing_white_space(lineStream);
                        if(check_error_create(lineStream.eof(), lineStream.fail(), name, type, x_location)){
                            //y_location
                            lineStream >> y_location;
                            trailing_white_space(lineStream);
                            if(check_error_create(lineStream.eof(), lineStream.fail(), name, type, y_location)){
                                //x_size
                                lineStream >> x_size;
                                trailing_white_space(lineStream);
                                    if(check_error_create(lineStream.eof(), lineStream.fail(), name, type, x_size)){
                                    //y_size
                                    lineStream >> y_size;
                                        trailing_white_space(lineStream);
                                        if(lineStream.fail()){
                                            error_cases(1, "NULL"); //line stream invalid
                                        }
                                        else if((type == "circle") && (x_size != y_size)){
                                            error_cases(6, "NULL");
                                        }
                                        else if(y_size < 0){
                                            error_cases(6, "NULL");
                                        }
                                        else if(!lineStream.eof()){
                                            error_cases(7, "NULL"); //too many args
                                        }
                                        else if(shapeCount >= max_shapes){ //the array is full
                                            error_cases(9, "NULL");
                                        }
                                        else{ //at this point we know that we have reached the end and that all of the inputs are correct, now go ahead and use the constructor
                                            shapesArray[shapeCount] = new Shape(name, type, x_location, x_size, y_location, y_size);
                                            cout << "Created" << " " << name << ":" << " "<< type << " " << x_location << " " << y_location << " " << x_size << " " << y_size << endl;
                                            shapeCount = shapeCount + 1; //increment number of shaped in array

                                        }
                                }
                            }
                        }
                    }
                }
            }
        }

        else if(command == "move"){
            bool error = false;
            string name;
            int loc1 = 0;
            int loc2 = 0;
            int loc3 = 0;

            if(lineStream.eof()){ 
                error_cases(8, "NULL");
                error = true;
            }
                if(!error){
                    lineStream >> name;
                    if(check_error_move(lineStream.eof(), lineStream.fail(), loc1, name)){
                        lineStream >> loc1;
                        if(check_error_move(lineStream.eof(), lineStream.fail(), loc1, name)){
                            lineStream >> loc2;
                            trailing_white_space(lineStream);
                            if(check_error_move(false, lineStream.fail(), loc2, name)){ //pass false for end of to check for the others first
                                if(!lineStream.eof()){
                                        error_cases(7, "NULL"); //too many args
                                }
                                else{
                                    execute_move(name, loc1, loc2);
                                    cout << "Moved" << " " << name << " " << "to" << " " << loc1 << " " << loc2 << endl;
                                }
                            }
                        }
                    }
                }
            
        }

        else if(command == "rotate"){
            bool error = false;
            int angle = 0;
            int angle2 = 0;
            string name;
            if(lineStream.eof()){
                error_cases(8, "NULL");
                error = true;
            }
            if(!error){
                lineStream >> name;
                if(check_error_rotate(lineStream.eof(), lineStream.fail(), angle, name)){
                    lineStream >> angle;
                    trailing_white_space(lineStream);
                    if(check_error_rotate(false, lineStream.fail(), angle, name)){
                        if(!lineStream.eof()){
                                error_cases(7, "NULL");
                        }
                        else{
                            execute_rotate(name, angle);
                        }
                    }

                }
            }
        }

        else if(command == "draw"){
            bool error = false;
            string draw_select;
            if(lineStream.eof()){
                error_cases(8, "NULL");
                error = true;
            }
            if(!error){
                lineStream >> draw_select;
                trailing_white_space(lineStream);
                if(check_error_draw(lineStream.fail(), draw_select)){
                    if(lineStream.eof()){
                        if(draw_select == "all"){
                            draw_all();
                        }
                        else{ //means that it was a name selected
                            draw_selected(draw_select);
                        }
                    }
                    else{
                        error_cases(7, "NULL"); //too many args
                    }
                }
            }
        }

        else if(command == "delete"){
            bool error = false;
            string delete_select;
            if(lineStream.eof()){
                error_cases(8, "NULL");
                error = true;
            }
            if(!error){
                lineStream >> delete_select;
                trailing_white_space(lineStream);
                if(check_error_draw(lineStream.fail(), delete_select)){
                    if(lineStream.eof()){
                        if(delete_select == "all"){
                            delete_all(true);
                        }
                        else{ //means that it was a name selected
                            delete_selected(delete_select);
                            cout << "Deleted shape" << " " << delete_select << endl;
                        }
                    }
                    else{
                        error_cases(7, "NULL"); //too many args
                    }
                }
            }
        }

        else{ //The command is not one of the ones listed
            error_cases(0, "NULL");
        }
        // Check for the command and act accordingly
        // ECE244 Student: Insert your code here

        //means the command was invalid as it is none of the listed ones
        //error accordingly

        // Once the command has been processed, prompt for the
        // next command
        cout << "> ";          // Prompt for input
        getline(cin, line);
    }  // End input loop until EOF.
    
    return 0;
}
void error_cases(int error_num, string name){
    if(error_num == 0){
        cout << "Error: invalid command" << endl;
        return;
    }
    else if(error_num == 1){
        cout << "Error: invalid argument" << endl;
        return;
    }
    else if(error_num == 2){
        cout << "Error: invalid shape name" << endl;
        return;
    }
    else if(error_num == 3){
        cout << "Error: shape" << " " << name << " " << "exists" << endl;
        return;
    }
    else if(error_num == 4){
        cout << "Error: shape" << " " << name << " " << "not found" << endl;
        return;
    }
    else if(error_num == 5){
        cout << "Error: invalid shape type" << endl;
        return;
    }
    else if(error_num == 6){
        cout << "Error: invalid value" << endl;
        return;
    }
    else if(error_num == 7){
        cout << "Error: too many arguments" << endl;
        return;
    }
    else if(error_num == 8){
        cout << "Error: too few arguments" << endl;
        return;
    }
    else if(error_num == 9){
        cout << "Error: shape array is full" << endl;
        return;
    }

}


void max_shape_in(int num_shapes){
    if(num_shapes < 0){
        error_cases(6, "NULL");
        return;
    }
    shapesArray = new Shape*[num_shapes]; //create the array of pointers
    for(int i = 0; i<num_shapes; i++){
        shapesArray[i] = NULL; // Initialize each pointer to NULL
    }
    cout << "New database: max shapes is" << " "<< num_shapes << endl;
}

bool check_error_create(bool endline, bool correct_type, string name, string type, int size_location_arg){
    if(correct_type){
        error_cases(1, "NULL"); //invalid argument
        return false;
    }
    else if(check_shape_name(name)){
        error_cases(2, "NULL");
        return false;
    }
    else if(!check_valid_name(name)){
        error_cases(3, name);
        return false;
    }
    else if(!check_shape_type(type)){
        error_cases(5, "NULL");
        return false;
    }
    else if(!loc_in_bounds(size_location_arg)){
        error_cases(6, "NULL");
        return false;
    }
    else if(endline){
        error_cases(8, "NULL"); //too few arguments 
        return false;
    }

    else{
        return true;
    }
}
bool check_shape_type(string type){
    if(type == "ellipse"){
        return true;
    }
    else if(type == "circle"){
        return true;
    }
    else if(type == "rectangle"){
        return true;
    }
    else if(type == "triangle"){
        return true;
    }
    else{
        return false;
    }
}

bool check_valid_name(string name){
    if(shapeCount != 0){ //means that there are no shapes
        for(int i = 0; i<max_shapes; i++){
            if(shapesArray[i] != NULL){
                if(shapesArray[i]->getName() == name){
                    return false;
                }
            }
        }
    }
    return true;
}

bool check_shape_name(string name){
    if(name == "maxShapes"){
        return true;
    }
    else if(name == "create"){
        return true;
    }
    else if(name == "move"){
        return true;
    }
    else if(name == "rotate"){
        return true;
    }
    else if(name == "draw"){
        return true;
    }
    else if(name == "delete"){
        return true;
    }
    else if(name == "ellipse"){
        return true;
    }
    else if(name == "circle"){
        return true;
    }
    else if(name == "rectangle"){
        return true;
    }
    else if(name == "triangle"){
        return true;
    }
    else if(name == "all"){
        return true;
    }
    else{
        return false;
    }
}

bool loc_in_bounds(int location){
    if(location < 0){
        return false;
    }
    else{
        return true;
    }
}

bool check_error_move(bool endline, bool correct_type, int location, string name){
    if(correct_type){
        error_cases(1, "NULL");
        return false;
    }
     else if(check_shape_name(name)){
        error_cases(2, "NULL");
        return false;
    }
    else if(check_valid_name(name)){
        error_cases(4, name);
        return false;
    }
    else if(!loc_in_bounds(location)){
        error_cases(6, "NULL");
        return false;
    }
    else if(endline){
        error_cases(8, "NULL");
        return false;
    }
    else{
        return true;
    }

}

void execute_move(string name, int loc1, int loc2){
    for(int i = 0; i<max_shapes; i++){
        if(shapesArray[i] != NULL){
            if(shapesArray[i]->getName() == name){
                shapesArray[i]->setName(name);
                shapesArray[i]->setXlocation(loc1);
                shapesArray[i]->setYlocation(loc2);
                return;
            }
        }
    }
    return;
}

//helpers for rotate
bool rotate_in_bounds(int angle){
    if(angle < 0 || angle > 360){
        return false;
    }
    else{
        return true;
    }
}

bool check_error_rotate(bool endline, bool correct_type, int angle, string name){
    if(correct_type){
        error_cases(1, "NULL");
        return false;
    }
    else if(check_shape_name(name)){
        error_cases(2, "NULL");
        return false;
    }
    else if(check_valid_name(name)){
        error_cases(4, name);
        return false;
    }
    else if(!rotate_in_bounds(angle)){
        error_cases(6, "NULL");
        return false;
    }
    else if(endline){
        error_cases(8, "NULL");
        return false;
    }else{
        return true;
    }
}

void execute_rotate(string name, int angle){
    for(int i = 0; i<max_shapes; i++){
        if(shapesArray[i] != NULL){
            if(shapesArray[i]->getName() == name){
                shapesArray[i]->setRotate(angle);
                cout << "Rotated" << " " << name << " " << "by" << " " << angle << " " << "degrees" << endl;
                return;
            }
        }
    }
    return;
}

//draw and delete helpers
bool check_error_draw(bool correct_type, string input_arg){
    if(correct_type){
        error_cases(1, "NULL");
        return false;
    }
    else if(check_shape_name(input_arg) && (input_arg != "all")){
        error_cases(2, "NULL");
        return false;
    }
    else if(check_valid_name(input_arg) && input_arg != "all"){
        error_cases(4, input_arg);
        return false;
    }
    else{
        return true;
    }
}

void draw_all(){
    cout << "Drew all shapes" << " ";
    for(int i = 0; i < max_shapes; i++){
        if(shapesArray[i] != NULL){
            shapesArray[i]->draw();
            cout << " ";
        }
    }
    cout << endl; // we want draw done in a row
    return;
}
void draw_selected(string name){
    for(int i = 0; i<max_shapes; i++){
        if(shapesArray[i] != NULL){
            if(shapesArray[i]->getName() == name){
                cout << "Drew" << " ";
                shapesArray[i]->draw();
                cout << endl;
                return;
            }
        }
    }
    cout << endl; //we dont end line in draw
    return;
}
void delete_all(bool output_deleted){
    for(int i = 0; i < max_shapes; i++){
        if(shapesArray[i] != NULL){ //WE DONT WANT TO DELETE NULL MEMBERS
            delete shapesArray[i];
            shapesArray[i] = NULL; //set deleted pointere to NULL
        }
    }
    if(output_deleted){
        cout << "Deleted: all shapes" << endl;
    }
    shapeCount = 0;
    return;
}
void delete_selected(string name){
    for(int i = 0; i < max_shapes; i++){
        if(shapesArray[i] != NULL){
            if(shapesArray[i]->getName() == name){
                delete shapesArray[i];
                //
                shapesArray[i] = NULL; //set pointer to deleted object to point to NULL
                return;
            }
        }
    }
    return;
}

void trailing_white_space(stringstream& ss){
    if(!ss.fail() && !ss.eof()){
        while(ss.peek() == 32 && !ss.eof()){
            ss.ignore(1);
        }
    }
    return;
}