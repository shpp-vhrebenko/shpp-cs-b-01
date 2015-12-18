#include "strlib.h"
#include "filelib.h"
#include "simpio.h"
#include <iostream>
#include <cstdlib>
#include <cmath>
#include "tokenscanner.h"
#include "gwindow.h"
#include "gobjects.h"
#include "myvector.h"
#include "mystack.h"

using namespace std;

/*The function returns the edited user input formula.*/
string getInputFormula();

/*The editing function entered by the user formula.
 * @param - User-entered formula is presented as a string.
 */
string editInputFunction(string inputString);

/*The function checks whether a string of function syntax.
 * @param - The string that is entered by the user part of formulating.
 */
bool isFunctions(string scannerString);

/*The conversion function user-entered formulas in RPN.
 * @param- User-entered formula.
 */
MyVector<string> conversionFunctionInRPNformat(string inputFormula);

/*The function determines the priority of operations.
 *@param-character operation.
 */
int priority(string symbol);

/*The function of a variable in a formula.
 *@param - Vector<string>, user-entered formulas in RPN format.
 */
MyVector<string> finding_variables(MyVector<string> inputFormulaInPRNformat);

/*The function test is not met there is already a variable in the formula.
@param - string, The found variable.
@param - Vector<string>, Array variables found.
*/
bool finding_matching_variables(string elem,MyVector<string> variables);

/*Initialize the variables in the formula.
@param - Vector<string>,user-entered formulas in RPN format.
@param - string, variable.
@param - double, value variable.
 */
MyVector<string>  initializeVariableInFormula(MyVector<string> result,string sr,double a);

/*This function defines the syntax of user-entered formula should calculate the result,
 *  or plotting functions.
 * @param - Vector<string>,user-entered formulas in RPN format.
 * @param - string, The formula entered by the user in the usual format.
 * return - Returns formula with initialized variables.
 */
void handlerPath(MyVector<string> formulaInPRNformat,string inputFormula);

/*The function checks the variable 'x' in the formula.
 *@param - Vector<string>,user-entered formulas in RPN format.
 */
bool presence_variable_X(MyVector<string> formulaInPRNformat);

/*Draw the graph of the function.
 * @param - Vector<string>,user-entered formulas in RPN format.
 * @param - string, The formula entered by the user in the usual format.
*/
void creating_graph(MyVector<string> formulaInPRNformat,string inputFormula);

/*Function draw coordinate system.
* @param - GWindow, Subject to display graphics.
* @param - int, scale graphics function.
* return - pair<double, double>, The value of the initial coordinates.
*/
pair<double, double> drawCoordinateSystem(GWindow &graphicsWindow, int scale);

/*Calculation results entered by the user formula.
* @param - Vector<string>,user-entered formulas in RPN format.
*/
double calculationEquationPRN(MyVector<string> inputOpn);

/*The function checks whether a character is an operator.
* @param - string, the character you want to check.
*/
bool IsOperator(string str);



int main()
{


    while(true){
        // We obtain a formula (function) that the user enters.
        string inputFormula = getInputFormula();

        // Transform the formula into RPN.
        MyVector<string> inputFormulaInRPNformat = conversionFunctionInRPNformat(inputFormula);

        // We find in the formula variables and specify the name of the values that the user specifies.
        MyVector<string> formulaInRPNformat = finding_variables(inputFormulaInRPNformat);

        // The handler determines whether the path plotting functions
        //In the presence of the variable 'x' or the calculation result in its absence.
        handlerPath(formulaInRPNformat,inputFormula);

        if (!getYesOrNo("Do you want build ones more functions graphic? (y/n): "))
            break;
    }


    return 0;
}

// We obtain a formula (function) that the user enters.
string getInputFormula(){
    string result = "";
    string inputFormula = "";
    while(true){
        inputFormula = toLowerCase(getLine("Enter formula y = :"));
        if(inputFormula.length() != 0){
            break;
        }
        cout<<"Incorrect input format funktion.Try enter again."<<endl;
    }
    result = editInputFunction(inputFormula);
    return result;
}

//Edit the resulting formula (remove spaces, we arrange marks multiplication).
string editInputFunction(string inputString){
    string result = "";
    string scannerString = "";
    char previos;
    TokenScanner scanner(inputString);
    scanner.scanNumbers();
    while(scanner.hasMoreTokens()){
        scannerString = scanner.nextToken();
        previos = result[result.length()-1];
        if(scannerString != " "){
            // If the token is the letter
            if(isalpha(scannerString[0])&& scannerString.length() == 1){
                if(isdigit(previos)){
                    result += "*";
                    result += scannerString;
                }
                else if(!isdigit(previos) && !isalpha(previos)){
                    if(previos == '('){
                        result += scannerString;
                    }
                    else if(previos == ')'){
                        result += "*";
                        result += scannerString;
                    }
                    else{
                        result += scannerString;
                    }
                }
            }
            // If the token is "("
            else if(scannerString == "("){
                if(isalpha(previos) && !isalpha(result[result.length()-2])){
                    result += "*";
                    result += scannerString;
                }
                else if(isdigit(previos)){
                    result += "*";
                    result += scannerString;
                }
                else{
                    result += scannerString;
                }


            }
            // If the token is a number
            else if(stringIsReal(scannerString)){
                if(previos == ')'){
                    result += "*";
                    result += scannerString;
                }
                else{
                    result += scannerString;
                }

            }
            // If the token is a function
            else if(isFunctions(scannerString)){
                if(isalpha(previos) || isdigit(previos)){
                    result += "*";
                    result += scannerString;
                }
                else{
                    result += scannerString;
                }

            }
            else{
                result += scannerString;
            }
       }
    }

    cout<<"InputFunction : "<<result<<endl;
    return result;
}

// Checking a string function.
bool isFunctions(string scannerString){

    if(scannerString == "sin"){
        return true;
    }
    else if(scannerString == "cos"){
        return true;
    }
    else if(scannerString == "sqrt"){
        return true;
    }

    return false;
}

// Transform the formula into RPN.
MyVector<string> conversionFunctionInRPNformat(string inputFormula){
    MyStack <string> StackOperands;
    MyVector<string> result;
    string scannerString = "";
    //string numbe = "";
    TokenScanner scanner(inputFormula);
    scanner.scanNumbers();
    while(scanner.hasMoreTokens()){
        scannerString = scanner.nextToken();
        if(isdigit(scannerString[0])){
            result.add(scannerString);            
        }
        else if(isalpha(scannerString[0])&& scannerString.length() == 1){
            result.add(scannerString);            
        }
        else if(!StackOperands.empty() && scannerString== "("){
            StackOperands.push(scannerString);                       
        }
        else if(!StackOperands.empty() && scannerString== ")"){
            while(StackOperands.top() != "("){
                result.add(StackOperands.top());                
                StackOperands.pop();                
            }
            StackOperands.pop();           
        }
        else if(!StackOperands.empty() &&  priority(scannerString) <= priority(StackOperands.top()) ){
            string topSteck = StackOperands.top();            
            while(priority(scannerString) <= priority(topSteck) && !StackOperands.empty()){
                topSteck = StackOperands.top();
                if(topSteck == "("){
                    break;
                }
                result.add(topSteck);
                StackOperands.pop();
            }
            StackOperands.push(scannerString);
        }
        else if(scannerString == "sqrt" || scannerString == "sin" || scannerString == "cos"){
            StackOperands.push(scannerString);            
        }
        else{
            StackOperands.push(scannerString);                       
        }


    }
    while(!StackOperands.empty()){
        if(StackOperands.top()!= "(" && StackOperands.top() != ")"){
            result.add(StackOperands.top());
            StackOperands.pop();
        }
        else{
            result.clear();
            result.add( "Default notation!!!");
            break;
        }

    }
    //cout<<"Function conversion in RPN format :"<<result<<endl;
    return result;
}

// Defines the value of the priority of certain operations.
int priority(string symbol)
{
    if (symbol == "(" || symbol == ")")
    {
        return 0;
    }
    if (symbol == "+" || symbol == "-")
    {
        return 1;
    }
    if (symbol == "*" || symbol == "/")
    {
        return 2;
    }
    if (symbol == "^")
    {
        return 3;
    }
    if (symbol == "sqrt" || symbol == "sin" || symbol == "cos")
    {
        return 4;
    }
    return -1;
}

// Find the variables in formule.I request the user variable values.
MyVector<string> finding_variables(MyVector<string> inputFormulaInPRNformat){
    MyVector<string> result = inputFormulaInPRNformat;
    string elem = "";
    MyVector<string> variables;
    variables.add("variables");
    double findVar = 0;
    for(int i(0);i<inputFormulaInPRNformat.size();i++){
        elem = inputFormulaInPRNformat[i];
        if(isalpha(elem[0]) && elem.length() == 1){
            if(elem != "x" && !finding_matching_variables(elem,variables)){
                cout<<"Enter value "<<elem<<" :";
                cin>>findVar;
                result = initializeVariableInFormula(result,elem,findVar);

                variables.add(elem);
            }
        }


    }
    return result;
}

// Checking for repetition variable in the formula.
bool finding_matching_variables(string elem,MyVector<string> variables){
    for(int i(0);i<variables.size();i++){
        if(variables[i] == elem){
            return true;
        }
    }
    return false;
}

// Initialize a variable value in the formula.
MyVector<string> initializeVariableInFormula(MyVector<string> result,string sr,double a){
    string elem = "";
    for(int i(0);i<result.size();i++){
        if(result[i] == sr){
            elem = realToString(a);
            result[i] = elem;
        }
    }
    return result;
}

// The handler determines whether the path plotting function
//In the presence of the variable 'x' or the calculation result in its absence.
void handlerPath(MyVector<string> formulaInPRNformat,string inputFormula){
    if(presence_variable_X(formulaInPRNformat)){
        creating_graph(formulaInPRNformat,inputFormula);
    }
    else{
        double result = calculationEquationPRN(formulaInPRNformat);
        cout << "result = "<< result << endl;
    }
}

// Checking for the presence of variable 'x' in the formula.
bool presence_variable_X(MyVector<string> formulaInPRNformat){
    bool present = false;
    for(int i(0);i<formulaInPRNformat.size();i++){
        if(formulaInPRNformat[i] == "x"){
            present = true;
        }

    }
    return present;
}

// Drawing coordinate axes.
pair<double, double> drawCoordinateSystem(GWindow &graphicsWindow,int scale) {
    pair<double, double> result;
    result.first = graphicsWindow.getWidth() / 2;
    result.second = graphicsWindow.getHeight() / 2;
    graphicsWindow.drawLine(result.first, 0, result.first, result.second * 2);
    graphicsWindow.drawLine(0, result.second, result.first * 2, result.second);

    GLabel* xMark = new GLabel("x", result.first * 2 - 20, result.second + 15);
    graphicsWindow.add(xMark);
    GLabel* yMark = new GLabel("y", result.first + 10, 20);
    graphicsWindow.add(yMark);
    return result;
}

// Drawing graphics features introduced. Before these requested maximum and minimum value 'x'.
void creating_graph(MyVector<string> formulaInRPNformat,string inputFormula){
    double xMin = getReal("Enter the left arguments scope value(-20 to 20): x min = ");
    double xMax = getReal("Enter the right arguments scope value(-20 to 20): x max = ");
    GWindow graphicsWindow(800, 600);
    graphicsWindow.setColor("BLACK");
    int scale = 20;
    pair<double, double> center = drawCoordinateSystem(graphicsWindow, scale);
    GLabel* formulaMark = new GLabel("y = " + inputFormula, 20, 30);
    formulaMark->setFont("Courier New");
    graphicsWindow.add(formulaMark);
    graphicsWindow.setColor("RED");
    string str = "x";
    double prewX = xMin;
    MyVector<string> formulaPRN = initializeVariableInFormula(formulaInRPNformat,str,prewX);
    double prewY = calculationEquationPRN(formulaPRN);
    for (double x = xMin + 0.01; x <= xMax; x += 0.01) {
        formulaPRN = initializeVariableInFormula(formulaInRPNformat,str,x);
        double y = calculationEquationPRN(formulaPRN);;
        graphicsWindow.drawLine(center.first + prewX * scale, center.second - prewY * scale,
                                center.first + x * scale, center.second - y * scale);
        prewX = x;
        prewY = y;
    }
}

// The calculation expression is written in reverse Polish notation.
double calculationEquationPRN(MyVector<string> inputOpn){
    double result = 0;
    MyStack <double> temp;
    double a = 0.0;
    double b = 0.0;
    string elemVector = "";
    for (int i = 0; i < inputOpn.size(); i++) //For each element in the vector
    {
        elemVector = inputOpn[i];
        //If the character - a figure that read and record all the numbers on the top of the stack
        if (stringIsReal(inputOpn[i]))  {
            temp.push(stringToReal(inputOpn[i])); //Write to the stack
        }
        //If the character - the operator
        else if (IsOperator(inputOpn[i]))
        {
            //Take one last value from the stack
            a = temp.top();
            temp.pop();
            if(inputOpn[i] == "sin"){
                result = sin(a);
            }
            else if(inputOpn[i] == "cos"){
                result = cos(a);
            }
            else if(inputOpn[i] == "sqrt"){
                result = sqrt(a);
            }
            else{
                //Take the last two values from the stack
                if(!temp.empty()){
                    b = temp.top();
                    temp.pop();

                    if(inputOpn[i] == "+"){
                        result = (b + a);
                    }
                    else if(inputOpn[i] == "-"){
                        result = (b - a);
                    }
                    else if(inputOpn[i] == "*"){
                        result = (b * a);
                    }
                    else if(inputOpn[i] == "/"){
                        if(b == 0){
                            cout<<"Incorrectly entered the equation. Division by zero!!!"<<endl;
                            cout<<"Enter the correct value of the dividend: ";
                            cin>>b;
                            result = (b / a) ;

                        }
                        else{
                        result = (b / a);
                        }
                    }
                    else if(inputOpn[i] == "^"){
                        result = pow(b,a);
                    }
                }
                else{
                    cout<<"The number of operations exceeds the number of operands. "<<endl;
                    cout<<"Therefore it is impossible to deduce the correct result. "<<endl;
                    cout<<"Please enter the correct formula."<<endl;
                    result = 0.0;
                    temp.push(result);
                    break;
                }
            }
            temp.push(result); //Calculation result is written back into the stack
        }
    }
    //we derive the value of the stack which is a solution
    result = temp.top();
    //cout<<temp.top()<<endl;
    return result;
}

// Check whether the element of the vector operator.
bool IsOperator(string str){
    if(str == "-"|| str == "+"){
        return true;
    }
    else if(str == "*"|| str == "/"){
        return true;
    }
    else if(str == "^"){
        return true;
    }
    else if(str == "sqrt"|| str == "sin" || str == "cos"){
        return true;
    }
    return false;

}


























