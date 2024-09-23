#include "assemblerMain.h"

int main(int argc, char *argv[])
{
    /* char *file_name[] = {"input_file1","input_file11", "input_file2", "input_file3", "input_file4",
                         "input_file5", "input_file6", "input_file7"}; */
    int i;
    for (i = 1; i < argc; ++i)
    {
        LabelType labelType;
        LabelNode headLableNode;
	    LineInfo headLinedata;
        char *output_address;
        output_address=process_file_pre_assembler(argv[i]);
        headLableNode = NULL;
        headLinedata = NULL;
	    labelType = NULL;
        process_file(output_address, &headLableNode, &headLinedata, &labelType);
        free_labels_list(headLableNode) ;
    }
    
    

    return 0;
}
