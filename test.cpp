//
// Created by Justin on 2/4/2019.
//

int main{

        char sentence[] = "addi $t7,$t6,1";
        char str1[20];
        char str2[20];
        char str3[20];
        int d;
        int match;
        match = sscanf(sentence, "%s $%s,$%s,%d", str1, str2, str3, &d);
        sscanf(str2, "$%s,$%s,%d", str2, str3, &d);
        printf("%s %s", str1, str2);
        printf("\n # match is: %d", match);
}

