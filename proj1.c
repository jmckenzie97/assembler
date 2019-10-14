
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
 * Supported Instructions:
 * instruction type opcode
 * ADD R 32
 * ADDI I 8
 * NOR R 39
 * ORI I 13
 * SLL R 0
 * LUI I 15
 * SW I 43
 * LW I 35
 * BNE I 5
 * J J 2
 *
 * Registers:
 * $t0-$t7 8-15
 * $s0-$s7 16-23
 * $0 0
 *
 * Instruction format:
 * R: OP rd, rs, rt
 * I: OP rt, IMM(rs)
 */

struct instruction{
    char* instr;
    char* param;
    char* label;
    int address;

    int laflag;

    int opCode;
    int rs;
    int rt;
    int rdorI;
    int final;
};


void initializeArr(struct instruction allInstr[100]);
struct instruction resolveLui(struct instruction instr);
struct instruction resolveOri(struct instruction instr);
int resolveOpCode(char* instr);
struct instruction resolveRegisters(struct instruction instr, struct instruction allInstr[100]);
int regCheck(char* regString);
int getLabelAddr(char* labelName, struct instruction allInstr[100]);


int main(int argc, char *argv[]) {
    freopen("C:\\Users\\Justin\\CLionProjects\\assembler\\mytest3.asm","r",stdin);
    char input[100];
    struct instruction allInstr[100];
    initializeArr(allInstr);
    int modifier = 0;
    while(fgets(input, 100, stdin)){            //loops til end of file to get and parse all of file
        char* label = NULL;
        char* instr = NULL;
        char* param = NULL;
        int space = 0;
        label = (char*)malloc((strlen(input)+1) * sizeof(char));        //various scanf statements for different line possibilities
        instr = (char*)malloc((strlen(input)+1) * sizeof(char));
        param = (char*)malloc((strlen(input)+1) * sizeof(char));

        if(sscanf(input, ".%s", label) == 1){
            continue;

        }
        else if(sscanf(input, "%s $%s", instr, param) == 2){    //accounts for a normal line with an instruction and a parameter
            int i = 0;
            for(i; i < 100; i++){
                if(allInstr[i].instr == NULL){
                    break;
                }
            }
            char la[] = "la";
            if(strcmp(instr, la) == 0){     //if there is an la instruction break it up into ori and lui
                allInstr[i].instr = (char*)malloc(10 * sizeof(char));   //dynamically allocate strings for struct
                allInstr[i].param = (char*)malloc(25 * sizeof(char));
                strcpy(allInstr[i].instr, "lui");
                strcpy(allInstr[i].param, param);
                allInstr[i].address = i*4 + modifier;
                allInstr[i] = resolveLui(allInstr[i]);
                allInstr[i].opCode = resolveOpCode(allInstr[i].instr);
                allInstr[i+1].instr = (char*)malloc(10 * sizeof(char));
                allInstr[i+1].param = (char*)malloc(25 * sizeof(char));
                strcpy(allInstr[i+1].instr, "ori");
                strcpy(allInstr[i+1].param, param);
                allInstr[i+1].address = (i+1)*4 + modifier;
                allInstr[i+1] = resolveOri(allInstr[i+1]);
                allInstr[i+1].opCode = resolveOpCode(allInstr[i+1].instr);

                allInstr[i].laflag = 1;
                allInstr[i+1].laflag = 1;
            }
            else{
                allInstr[i].instr = (char*)malloc(10 * sizeof(char));
                allInstr[i].param = (char*)malloc(25 * sizeof(char));
                strcpy(allInstr[i].instr, instr);
                strcpy(allInstr[i].param, param);
                allInstr[i].address = i*4 + modifier;
                allInstr[i].opCode = resolveOpCode(allInstr[i].instr);
            }

        }
        else if(sscanf(input, "%s %s $%s", label, instr, param) == 3){ //accounts for a line with a label, instruction and parameter
            int i = 0;
            for(i; i < 100; i++){
                if(allInstr[i].instr == NULL){
                    break;
                }
            }
            char la[] = "la";
            if(strcmp(instr, la) == 0){
                allInstr[i].instr = (char*)malloc(10 * sizeof(char));
                allInstr[i].param = (char*)malloc(25 * sizeof(char));
                strcpy(allInstr[i].instr, "lui");
                strcpy(allInstr[i].param, param);
                allInstr[i].address = i*4 + modifier;
                allInstr[i] = resolveLui(allInstr[i]);
                allInstr[i].opCode = resolveOpCode(allInstr[i].instr);
                allInstr[i+1].instr = (char*)malloc(10 * sizeof(char));
                allInstr[i+1].param = (char*)malloc(25 * sizeof(char));
                strcpy(allInstr[i+1].instr, "ori");
                strcpy(allInstr[i+1].param, param);
                allInstr[i+1].address = (i+1)*4 + modifier;
                allInstr[i+1] = resolveOri(allInstr[i+1]);
                allInstr[i+1].opCode = resolveOpCode(allInstr[i+1].instr);
                allInstr[i].label = (char*)malloc(10 * sizeof(char));
                allInstr[i+1].label = (char*)malloc(10 * sizeof(char));
                strcpy(allInstr[i].label, label);
                strcpy(allInstr[i+1].label, label);

                allInstr[i].laflag = 1;
                allInstr[i+1].laflag = 1;
            }
            else{
                allInstr[i].instr = (char*)malloc(10 * sizeof(char));
                allInstr[i].param = (char*)malloc(25 * sizeof(char));
                strcpy(allInstr[i].instr, instr);
                strcpy(allInstr[i].param, param);
                allInstr[i].address = i*4 + modifier;
                allInstr[i].opCode = resolveOpCode(allInstr[i].instr);
                allInstr[i].label = (char*)malloc(10 * sizeof(char));
                strcpy(allInstr[i].label, label);
            }


        }
        else if(sscanf(input, "%s .%s %d", label, instr, &space) == 3) { //accounts for lines with directives like .space
            int i = 0;
            for(i; i < 100; i++){
                if(allInstr[i].instr == NULL){
                    break;
                }
            }
            char w[] = "word";
            if(strcmp(instr, w) != 0) {
                allInstr[i].label = (char *) malloc(25 * sizeof(char));
                strcpy(allInstr[i].label, label);
                allInstr[i].instr = (char *) malloc(10 * sizeof(char));
                strcpy(allInstr[i].instr, instr);
                allInstr[i].address = i * 4 + modifier;
                modifier += space;
            }
            else{
                allInstr[i].label = (char *) malloc(25 * sizeof(char));
                strcpy(allInstr[i].label, label);
                allInstr[i].instr = (char *) malloc(10 * sizeof(char));
                strcpy(allInstr[i].instr, instr);
                allInstr[i].address = i * 4 + modifier;
                modifier += 4;
            }

        }
        else if(sscanf(input, "%s %s", instr, param) == 2){     //for jump only
            int i = 0;
            for(i; i < 100; i++){
                if(allInstr[i].instr == NULL){
                    break;
                }
            }
            allInstr[i].instr = (char*)malloc(10 * sizeof(char));
            allInstr[i].param = (char*)malloc(25 * sizeof(char));
            strcpy(allInstr[i].instr, instr);
            strcpy(allInstr[i].param, param);
            allInstr[i].address = i*4 + modifier;
            allInstr[i].opCode = resolveOpCode(allInstr[i].instr);
        }
        else if(sscanf(input, "%s:", label) == 1){   //accounts for lines with only a label
            if(label[0] == '.'){
                continue;
            }
            int i = 0;
            for(i; i < 100; i++){
                if(allInstr[i].instr == NULL){
                    break;
                }
            }
            allInstr[i].instr = (char*)malloc(10 * sizeof(char));
            strcpy(allInstr[i].instr, "label");
            allInstr[i].label = (char*)malloc(10 * sizeof(char));
            strcpy(allInstr[i].label, label);
            allInstr[i].address = i*4 + modifier;
        }

        int i = 0;

        free(instr);
        free(label);
        free(param);
    }

    int i = 0;
    for(i; i < 100; i++){ //loop for the second pass to actually create the final output
        if((allInstr[i].opCode == 32 || allInstr[i].opCode == 39) && allInstr[i].instr != NULL){

            allInstr[i] = resolveRegisters(allInstr[i], allInstr);

            allInstr[i].rs = allInstr[i].rs << 21;
            allInstr[i].rt = allInstr[i].rt << 16;
            allInstr[i].rdorI = allInstr[i].rdorI << 11;
            allInstr[i].final = allInstr[i].rs + allInstr[i].rt + allInstr[i].rdorI + allInstr[i].opCode;
            printf("0x%08X : 0x%08X\n", allInstr[i].address, allInstr[i].final);

        }
        else if(allInstr[i].opCode == 0 && allInstr[i].instr != NULL){
            allInstr[i] = resolveRegisters(allInstr[i], allInstr);

            allInstr[i].rt = allInstr[i].rt << 16;
            allInstr[i].rdorI = allInstr[i].rdorI << 11;
            allInstr[i].rs = allInstr[i].rs << 6;           //in the case of sll, the rs variable functions as shamt
            allInstr[i].final = allInstr[i].rt + allInstr[i].rdorI + allInstr[i].rs + allInstr[i].opCode;
            printf("0x%08X : 0x%08X\n", allInstr[i].address, allInstr[i].final);
        }
        else if((allInstr[i].opCode == 8 || allInstr[i].opCode == 13 || allInstr[i].opCode == 43 || allInstr[i].opCode == 35) && allInstr[i].instr != NULL){
            allInstr[i] = resolveRegisters(allInstr[i], allInstr);

            allInstr[i].opCode = allInstr[i].opCode << 26;
            allInstr[i].rs = allInstr[i].rs << 21;
            allInstr[i].rt = allInstr[i].rt << 16;
            allInstr[i].final = allInstr[i].opCode + allInstr[i].rs + allInstr[i].rt + allInstr[i].rdorI;
            printf("0x%08X : 0x%08X\n", allInstr[i].address, allInstr[i].final);
        }
        else if((allInstr[i].opCode == 15) && allInstr[i].instr != NULL){
            allInstr[i] = resolveRegisters(allInstr[i], allInstr);

            allInstr[i].opCode = allInstr[i].opCode << 26;
            allInstr[i].rt = allInstr[i].rt << 16;
            allInstr[i].final = allInstr[i].opCode + allInstr[i].rt + allInstr[i].rdorI;
            printf("0x%08X : 0x%08X\n", allInstr[i].address, allInstr[i].final);
        }
        else if((allInstr[i].opCode == 2) && allInstr[i].instr != NULL){
            allInstr[i] = resolveRegisters(allInstr[i], allInstr);

            allInstr[i].opCode = allInstr[i].opCode << 26;
            allInstr[i].final = allInstr[i].opCode + allInstr[i].rdorI;
            printf("0x%08X : 0x%08X\n", allInstr[i].address, allInstr[i].final);
        }
        else if((allInstr[i].opCode == 5) && allInstr[i].instr != NULL){
            allInstr[i] = resolveRegisters(allInstr[i], allInstr);

            allInstr[i].opCode = allInstr[i].opCode << 26;
            allInstr[i].rs = allInstr[i].rs << 21;
            allInstr[i].rt = allInstr[i].rt << 16;
            allInstr[i].final = allInstr[i].opCode + allInstr[i].rs + allInstr[i].rt + allInstr[i].rdorI;
            printf("0x%08X : 0x%08X\n", allInstr[i].address, allInstr[i].final);
        }


    }

    return 0;

}

void initializeArr(struct instruction allInstr[100]){   //function that loops through the array and initializes some variables
    int i = 0;
    for(i; i < 100; i++){
        allInstr[i].instr = NULL;
        allInstr[i].param = NULL;
        allInstr[i].label = NULL;
        allInstr[i].laflag = 0;
        allInstr[i].opCode = 1000;
    }
}

struct instruction resolveLui(struct instruction instr){    //function to help with the la expansion
    char* reg1 = NULL;
    char* label = NULL;

    reg1 = (char*)malloc(4 * sizeof(char));
    label = (char*)malloc(strlen(instr.param) * sizeof(char));

    strncpy(reg1, instr.param, 2);
    reg1[2] = '\0';
    strcpy(label, &instr.param[3]);
    sprintf(instr.param, "$1,%s,%s", reg1,label);

    free(reg1);
    free(label);
    return instr;
}

struct instruction resolveOri(struct instruction instr){ //function to help with the la expansion
    char* reg1 = NULL;
    char* label = NULL;

    reg1 = (char*)malloc(4 * sizeof(char));
    label = (char*)malloc(strlen(instr.param) * sizeof(char));

    strncpy(reg1, instr.param, 2);
    reg1[2] = '\0';
    strcpy(label, &instr.param[3]);
    sprintf(instr.param, "%s,$1,%s", reg1,label);
    free(reg1);
    free(label);
    return instr;
}

int resolveOpCode(char* instr){     //function that takes an instruction in and returns the opcode as an int

    if(strcmp(instr, "add") == 0){
        return 32;
    }
    else if(strcmp(instr, "addi") == 0){
        return 8;
    }
    else if(strcmp(instr, "nor") == 0){
        return 39;
    }
    else if(strcmp(instr, "ori") == 0){
        return 13;
    }
    else if(strcmp(instr, "sll") == 0){
        return 0;
    }
    else if(strcmp(instr, "lui") == 0){
        return 15;
    }
    else if(strcmp(instr, "sw") == 0){
        return 43;
    }
    else if(strcmp(instr, "lw") == 0){
        return 35;
    }
    else if(strcmp(instr, "bne") == 0){
        return 5;
    }
    else if(strcmp(instr, "j") == 0){
        return 2;
    }
    return -1;
}

struct instruction resolveRegisters(struct instruction instr, struct instruction allInstr[100]){
    if(instr.opCode == 32 || instr.opCode == 39){ //function to help resolve the string registers into their respective ints
        if(strstr(instr.param, "$0") == NULL) {
            char *temp = NULL;
            temp = (char *) malloc(strlen(instr.param) * sizeof(char));
            strncpy(temp, instr.param, 2);
            temp[2] = '\0';
            instr.rdorI = regCheck(temp);
            strncpy(temp, &instr.param[4], 2);
            temp[2] = '\0';
            instr.rs = regCheck(temp);
            strncpy(temp, &instr.param[8], 2);
            temp[2] = '\0';
            instr.rt = regCheck(temp);
            free(temp);
            return instr;
        }
        else{
            char *temp = NULL;
            temp = (char *) malloc(strlen(instr.param) * sizeof(char));
            strncpy(temp, instr.param, 2);
            temp[2] = '\0';
            instr.rdorI = regCheck(temp);
            strncpy(temp, &instr.param[4], 2);
            temp[2] = '\0';
            instr.rs = regCheck(temp);
            strncpy(temp, &instr.param[8], 2);
            temp[2] = '\0';
            instr.rt = regCheck(temp);
            free(temp);
            return instr;
        }

    }
    else if (instr.opCode == 8 || instr.opCode == 13){
        if(instr.laflag != 1) {
            if(strstr(instr.param, "$0") == NULL) {
                char *temp = NULL;
                temp = (char *) malloc(20 * sizeof(char));
                strncpy(temp, instr.param, 2);
                temp[2] = '\0';
                instr.rt = regCheck(temp);
                strncpy(temp, &instr.param[4], 2);
                temp[2] = '\0';
                instr.rs = regCheck(temp);
                strcpy(temp, &instr.param[7]);
                instr.rdorI = atoi(temp);
                free(temp);
                return instr;
            }
            else{
                char *temp = NULL;
                temp = (char *) malloc(20 * sizeof(char));
                strncpy(temp, instr.param, 2);
                temp[2] = '\0';
                instr.rt = regCheck(temp);
                strncpy(temp, &instr.param[4], 2);
                temp[2] = '\0';
                instr.rs = regCheck(temp);
                strcpy(temp, &instr.param[6]);
                instr.rdorI = atoi(temp);
                free(temp);
                return instr;
            }
        }
        else
        {
            char *temp = NULL;
            temp = (char *) malloc(strlen(instr.param) * sizeof(char));
            strncpy(temp, instr.param, 2);
            temp[2] = '\0';
            instr.rt = regCheck(temp);
            strncpy(temp, &instr.param[3], 1);
            temp[1] = '\0';
            instr.rs = regCheck(temp);
            strcpy(temp, &instr.param[6]);
            int tempInt = getLabelAddr(temp, allInstr);
            instr.rdorI =  tempInt & 0x8ff;
            free(temp);
            return instr;
        }
    }
    else if (instr.opCode == 43 || instr.opCode == 35){
        char *temp = NULL;
        temp = (char*)malloc(strlen(instr.param) * sizeof(char));
        strncpy(temp, instr.param, 2);
        temp[2] = '\0';
        instr.rt = regCheck(temp);
        int i = 0;
        for(i; i < strlen(instr.param); i++){
            if(instr.param[i] == '(')
                break;
        }
        free(temp);
        temp = (char*)malloc(strlen(instr.param) * sizeof(char));
        strncpy(temp, &instr.param[3], i-3);
        strcat(temp, "\0");
        instr.rdorI = atoi(temp);
        free(temp);
        temp = (char*)malloc(strlen(instr.param) * sizeof(char));
        strncpy(temp, &instr.param[i+2], 2);
        temp[2] = '\0';
        instr.rs = regCheck(temp);
        free(temp);
        return instr;

    }
    else if (instr.opCode == 0){
        char *temp = NULL;
        temp = (char*)malloc(strlen(instr.param) * sizeof(char));
        strncpy(temp, instr.param, 2);
        temp[2] = '\0';
        instr.rdorI = regCheck(temp);
        strncpy(temp, &instr.param[4], 2);
        temp[2] = '\0';
        instr.rt = regCheck(temp);
        strcpy(temp, &instr.param[8]);
        strcat(temp, "\0");
        instr.rs = atoi(temp);
        free(temp);
        return instr;

    }
    else if (instr.opCode == 15){
        if(instr.laflag != 1) {
            char *temp = NULL;
            temp = (char *) malloc(strlen(instr.param) * sizeof(char));
            strncpy(temp, instr.param, 2);
            temp[2] = '\0';
            instr.rt = regCheck(temp);
            strcpy(temp, &instr.param[4]);
            instr.rdorI = atoi(temp);
            free(temp);
            return instr;
        }
        else{
            char *temp = NULL;
            temp = (char *) malloc(strlen(instr.param) * sizeof(char));
            strncpy(temp, instr.param, 2);
            temp[2] = '\0';
            instr.rt = regCheck(temp);
            strcpy(temp, &instr.param[6]);
            instr.rdorI = getLabelAddr(temp, allInstr) >> 16;
            free(temp);
            return instr;

        }
    }
    else if (instr.opCode == 2){
        int tempAddr = 0;
        tempAddr = getLabelAddr(instr.param, allInstr);
        instr.rdorI = instr.address >> 26;
        instr.rdorI += tempAddr << 2;
        return instr;

    }
    else if (instr.opCode == 5){
        char *temp = NULL;
        temp = (char *) malloc(20 * sizeof(char));
        strncpy(temp, instr.param, 2);
        temp[2] = '\0';
        instr.rs = regCheck(temp);
        strncpy(temp, &instr.param[4], 2);
        temp[2] = '\0';
        instr.rt = regCheck(temp);
        strcpy(temp, &instr.param[7]);
        int tempAddr = 0;
        tempAddr = getLabelAddr(temp, allInstr);     //need diff between instruction address and label address
        instr.rdorI = tempAddr - instr.address;
        free(temp);
        return instr;
    }

}
int regCheck(char* regString){ //function that takes in a register value as a string and returns it as its respective int
    if(regString[0] == 't'){
        if(regString[1] == '0'){
            return 8;
        }
        else if(regString[1] == '1'){
            return 9;
        }
        else if(regString[1] == '2'){
            return 10;
        }
        else if(regString[1] == '3'){
            return 11;
        }
        else if(regString[1] == '4'){
            return 12;
        }
        else if(regString[1] == '5'){
            return 13;
        }
        else if(regString[1] == '6'){
            return 14;
        }
        else if(regString[1] == '7'){
            return 15;
        }
    }
    else if(regString[0] == 's'){
        if(regString[1] == '0'){
            return 16;
        }
        else if(regString[1] == '1'){
            return 17;
        }
        else if(regString[1] == '2'){
            return 18;
        }
        else if(regString[1] == '3'){
            return 19;
        }
        else if(regString[1] == '4'){
            return 20;
        }
        else if(regString[1] == '5'){
            return 21;
        }
        else if(regString[1] == '6'){
            return 22;
        }
        else if(regString[1] == '7'){
            return 23;
        }
    }
    else if(regString[0] == '$')
        return 1;
    else
        return 0;
}

int getLabelAddr(char* labelName, struct instruction allInstr[100]){

    char* temp = NULL;
    temp = (char *) malloc(strlen(labelName) * sizeof(char));
    strcpy(temp, labelName);
    strcat(temp, ":");
    int i = 0;
    for(i; i < 100; i++){
        if(allInstr[i].label == NULL)
        {
            continue;
        }
        else if(strcmp(temp, allInstr[i].label) == 0){
            return  allInstr[i].address;
        }
    }
}