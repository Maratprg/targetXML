#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>


/* The first words about our node
 *  - it is may has or hasnt childrens. and if node hasn't any childens he has a reserved 10 place
 *    for inserting childs, and it is own capacity (vacuum volume).
 *  - and after if you demand some new node-item place on a tree you gone wider our tree.
 *    and the mechanics that allow this to be done are contained in the special functions named with the tips Engine.
 *  - if your insert a simple tree node your change eternalCount.
 *    that because you need to be posible widing you tree.
 *  - making reEngine function better! take only changes not in current node
 *    but such of bit only!!! a underlaing chain! it is so important!
 *
 * Making a simple tree
 * To make a tree is better throw steps
 *     - create a simple node and constr with constructor(ptr, counter chains, value integer):
 *          struct Node* root;  root = Node_Cnstr_Empty(node, 0, 1);
 *     - and than create a nodes to attach childrens:
 *          struct Node* chld;  chld = Node_Cnstr_Empty(chld, 0, 1);
 *          Node_Attach_Child(root,chld, on wich chain attach integer);
 *
 * Content step.
 * Turn our tree has a content we need
 *  - construct our content declaration with head and belly char* ptr.
 *  - make allocate constructors and dealocate destructors.
 *  - we need perfectly deallocation mechanism (every steps valgrind checking).
 *  - include needable several libs to use our string length methods and others.
 *      When we has a content constructor we must understand that
 *      first element of our **ptr array named content is a tag value wich is
 *      in head char array(body we marked by zero), and each one time we adding a new content to **pointer
 *      we add atribute name and atribute value. And that we understant content is dynamicle must have
 *      at least one value that will be our tag. counter and capacity mech looks like cnstr before.
 *  - in this step so important to allow user make '\0' zero-terminated non content possibility.
 *
 *  Stream step.
 *  It is also recursion step. We allow our user to has a possibility to ride out stream in our
 *  bits of tree by the recursion. It is so much important and difficult step and it shown rather in a simplified version.
 *    - make a read-potencial stream.
 *    - make a such kind of buffer. widing mechanizm ...
 *    - making a file-storage store function is driving by recursion.
 *    - checking sure in creation, if you throw stream to function (Export_by_FILEFlow) make your own size buffer before (memXmlBuffer(size_t)).
 *      buffer must has allow-potencial to read you long strings or you programm will crash. create with a some kind of margin.
 *
 *  Reading XML step
 *  First thing first - making capacity-extendable structure to keep parse data! I am pictured those in structure named Xml_Buffer_Capacitor.
 *  In next step there are quite a few different versions of reading from a file.
 *  Here an important property is not to complicate but to make simple parsing in a loop.
 *  Although this is also possible with the help of recursion.
 *
 */


//global report variable and it report mech
static char reportS[255];
void my_Print_Hex(char* chr, int x) { printf("%s, %x \n", chr, x); return; }
void my_Print_Int(char* chr, int x) { printf("%s, %d \n", chr, x); return; }

    //bool if it will needable
    enum My_BOOL
    {
        FALSE,
        TRUE
    };

//*************** <Content> ******************
struct Content
{
    char* head;
    char* belly;

    size_t head_length;
    size_t belly_length
};

            struct Content* Content_Construct (struct Content* content, char* head, char* belly)
            {
                //build content space
                content = (struct Content*)malloc(sizeof(struct Content));

                //import strlengths
                content->head_length = strlen(head) + 1;
                content->belly_length = strlen(belly) + 1;

                //Poke memory
                content->head = (char*)malloc(content->head_length * sizeof(char)); //memLeak
                content->belly = (char*)malloc(content->belly_length * sizeof(char)); //memLeak


                if(strlen(head) != 0 && strlen(belly) != 0)
                {
                    content->head = strcpy(content->head, head);
                    content->belly = strcpy(content->belly, belly);
                }
                else if (strlen(head) == 0 && strlen(belly) != 0)
                {
                    content->head[0] = '\0';
                    content->belly = strcpy(content->belly, belly);
                }
                else if (strlen(head) != 0 && strlen(belly) == 0)
                {
                    content->head = strcpy(content->head, head);
                    content->belly[0] = '\0';
                }
                else{};

                printf("Content Report: construction of content %x \n", content);
                return content;
            };

            void Content_Destruct (struct Content* content)
            {
                printf ("Destruction of Content %x \n", content);
                free(content->head); //memLeak
                free(content->belly); //memLeak
                free(content);
                return;
            };

//************** </Content> ******************

struct Node nullNode; //this is a null marking structure

struct Node
{
    int value;
    size_t eternalCount; //chain counter
    size_t capacity; //reserved volume of chain
    size_t upyardPlaceHolder; //the place in upyard chain
    struct Node* federal;
     struct Node** chain;

    size_t content_capacity; //important content capacity
    size_t content_counter; //important content counter
     struct Content** content;
};

       //try to make an empty node
       struct Node* Node_Cnstr_Empty (struct Node* node, int val, char* tag, char* belly)
       {
                //Poke memory  (two pointers)
                node = (struct Node*) malloc(sizeof(struct Node));
                    node->capacity = 10;
                    node->chain = (struct Node**) malloc(node->capacity * sizeof(struct Node*));
                    for (int i=0; i<node->capacity; i++) node->chain[i] = &nullNode;

                //engage value
                node->value = val;

                //press up counter
                node->eternalCount = 0;

                //adress federal null
                node->federal = &nullNode;

                //make upyard value -1 unchained
                node->upyardPlaceHolder = -1;

                    //construct content
                    node->content_capacity = 10;
                    node->content = (struct Content**) malloc(node->content_capacity * sizeof(struct Content*));

                        //genegate empty tag content
                        struct Content* tg = Content_Construct(tg, tag, belly);
                        node->content[0] = tg;
                        node->content_counter = 1;

                my_Print_Hex("NODE_REPORT(cnstr): constructor empty throwing ", node);
                return node;
       };


       //try to Engine - Widing capacity
       struct Node* reEngineNode (struct Node* node)
       {
            if (node->capacity == node->eternalCount)
            {
                //Create temporary copy
                struct Node** tmp_chain = (struct Node**)malloc(node->capacity * sizeof(struct Node*));
                for (int i=0; i<node->eternalCount; i++)
                {
                    tmp_chain[i] = node->chain[i];
                }

                //engage capacity
                node->capacity = 2*node->capacity;

                //Poke(free) memory
                free(node->chain);
                node->chain = (struct Node**)malloc(node->capacity * sizeof(struct Node*));
                    for (int i=0; i<node->capacity; i++)
                    {
                        if (i < node->eternalCount)
                        {
                            node->chain[i] = tmp_chain[i];
                        }
                        else
                        {
                            node->chain[i] = &nullNode;
                            node->chain[i]->upyardPlaceHolder = i; //mayBeNeedable in debug
                        }
                    }

                //deleash temporary copy
                free(tmp_chain);

               //report
               my_Print_Hex("NODE_REPORT(engine): sucsessfull reengined node", node);

               //return engined node
               return node;
            }
            else
            {
               return node;
            };
       }


       //check reEngine and after that try to attaching child
       void Node_Attach_Child(struct Node* parent, struct Node* chld)
        {
            //check and get ready tree to engine
            if (parent->capacity == parent->eternalCount)
            {
                parent = reEngineNode(parent);
            };

                //adapt childrens (SIGNES::in all cases our eternalCount signs to needable range(0 to eternalCount-1))
                parent->chain[parent->eternalCount] = chld;
                chld->upyardPlaceHolder = parent->eternalCount;
                chld->federal = parent;

                    //attach_report
                    printf("NODE_REPORT(atch): attach report P %x", parent);
                    printf(" hook on Ch %x", chld);
                    printf(" placed in [%d", parent->eternalCount);
                    printf("] \n");

                //engage eternalCount
                parent->eternalCount++;
            return;
        }

        //check and resize content
        struct Node* Node_Content_Resize (struct Node* node)
        {
            if(node->content_counter == node->content_capacity)
            {
                printf("NODE REPORT(content_chk_and_rsz): resizing content node %x \n", node);

                //copy for static pointers array
                struct Content** temporary = (struct Content**) malloc(node->content_capacity * sizeof(struct Content*));
                for (int i=0; i < node->content_capacity; i++) temporary[i] = node->content[i];

                //engaging
                node->content_capacity = 2 * node->content_capacity;

                //reallocation
                free(node->content);
                node->content = (struct Content**) malloc (node->content_capacity * sizeof(struct Content*));

                //reini
                for (int i=0; i < node->content_capacity; i++)
                {
                    if (i < (node->content_capacity/2)) node->content[i] = temporary[i];
                    else node->content[i] = NULL;
                }

                //deleash memory
                free(temporary);
            };
            return node;
        };

        //content addition of atribute
        struct Node* Node_Add_Atribute (struct Node* node, char* head, char* belly)
        {
            struct Content* attr = Content_Construct(attr, head, belly); //memLeak?

            node = Node_Content_Resize(node);
                if (attr != NULL)
                {
                    node->content[node->content_counter] = attr;
                    node->content_counter++;
                }
                else
                {
                    printf("pragma! return virgin knot \n");
                }
            return node;
        };

        //content adobt body
        struct Node* Node_Adobt_Body (struct Node* node, char* compromete)
        {
            //find out size of compromete
            size_t comprometeLength = strlen(compromete);

            //engage capacity of char
            int counter = 1;
            while (node->content[0]->belly_length <= comprometeLength)
            {
                node->content[0]->belly_length = node->content[0]->belly_length * 2;
                counter++;
            }

            //refresh Memory for string
            if (counter > 1)
            {
                free(node->content[0]->belly);
                node->content[0]->belly = (char*) malloc (node->content[0]->belly_length * sizeof(char));
            }

            //data copy
            strcpy(node->content[0]->belly, compromete);

            return node;
        }


        //nodes destructor
        void Node_Destr_Curr(struct Node* node)
        {
                if (node->eternalCount > 0)
                {
                    for (int i=0; i<node->eternalCount; i++)
                    {
                        Node_Destr_Curr(node->chain[i]);
                    }
                    printf("NODE_REPORT_RCS(dstr): destroy node %x with value %d \n", node, node->value);

                                //deleaching content cycle
                                for (int j=0; j<node->content_counter; j++)
                                {
                                        Content_Destruct(node->content[j]);
                                };
                                free(node->content);  //MEM LEAK IS HERE!


                    free(node->chain);
                    free(node);

                }
                else
                {
                    printf("NODE_REPORT_RCS(dstr): destroy node %x with value %d \n", node, node->value);

                                //deleaching content cycle
                                for (int j=0; j<node->content_counter; j++)
                                {
                                        Content_Destruct(node->content[j]);
                                };
                                free(node->content);  //MEM LEAK IS HERE!

                    free(node->chain);
                    free(node);
                }
            return;
        }


        //subfunctional nodes
        void showCurrentNode(struct Node* node)
        {
             printf("SHOW(showCurrentNode): node adress %x", node);
                           printf(" node->federal %x", node->federal);
                           printf(" chain[0] %x", node->chain[0]);
                             printf(" eternalCount %d", node->eternalCount);
                             printf(" value %d \n", node->value);
                               for (int t=0; t<node->content_counter; t++)
                               {
                                 printf("\tcontent[%d] head - %s,  belly - %s \n", t, node->content[t]->head, node->content[t]->belly);
                               }


             return;
        }

        void showCurrentChain(struct Node* node)
                    {
                        if (node->eternalCount > 0)
                        {
                            for (int i=0; i<node->eternalCount; i++)
                            {
                                printf("\tSHOW(showCurrentChain): node->chain %d adress %x value %d \n", i,  node->chain[i], node->chain[i]->value);
                            }
                        }
                        else
                        {
                            printf("\tSHOW(showCurrentChain): sorry but node hasnt any child (eternalCount %d) \n", node->eternalCount);
                        }
                        return;
                    }




                    //making a tree steps
                    struct Node* proceed_Up(struct Node* node)
                    {
                        printf ("<<< proceed_Up >>> ");
                        if (node->federal != &nullNode) { node = node->federal; my_Print_Hex(" in_da_hood ", node); }
                        else  { printf("PRC_UP_ERR: sorry cant step upyard, i am exactly on root \n"); }
                        return node;
                    }

                    //making a steps downyard (iN here is a array index ranged from 0 to max < eternalCount)
                    struct Node* proceed_Down(struct Node* node, int iN)
                    {
                        if (node->eternalCount > 0)
                        {
                                printf ("<<<proceed_Down>>> ");
                                if (iN < node->eternalCount && iN >-1)
                                {
                                    if (node->chain[iN] == &nullNode)
                                    {
                                        printf("PRC_DWN_ERR: cant step downyard, current note chain exactly linked nullNode \n");
                                    }
                                    else
                                    {
                                        node = node->chain[iN];
                                        my_Print_Hex(" in_da_hood ", node);
                                    }
                                }
                                else
                                {
                                    printf("PRC_DWN_ERR: cant bind eternalCount and iN (out of range) \n");
                                }

                        }
                        else
                        {
                            printf("PRC_DWN_ERR: cant step downyard, empty Node! \n");
                        }
                        return node;
                    }



//RECURSION EXPORT ----------------------------
void Export_Screen_Int (struct Node* node)
{
   if (node->eternalCount > 0)
   {
        for(int i=0; i<node->eternalCount; i++)
        {
            Export_Screen_Int(node->chain[i]);
        }
        printf("EXPORT_SCR:: node adr %x value %d \n", node, node->value);
    }
    else
    {
        printf("EXPORT_SCR: node adr %x value %d \n", node, node->value);
        return;
    };
}

        static int tabs_shifter;

        void Export_Screen_Content_Tabs (struct Node* node)
        {
            if (node->eternalCount > 0)
            {
               //tabs_shifting
               for (int sh=0; sh<tabs_shifter; sh++) printf("\t");

               for (int t=0; t<node->content_counter; t++)
                   {
                        if (t==0) printf("<%s",node->content[t]->head);
                        else printf(" %s=\"%s\"",node->content[t]->head, node->content[t]->belly);
                   }
                   printf(">\n");

               tabs_shifter++;

               for(int i=0; i<node->eternalCount; i++)
                    {
                        Export_Screen_Content_Tabs(node->chain[i]); //RECURSIVE STEP
                    }
                        tabs_shifter--;
                        //tabs_shifting
                        for (int sh=0; sh<tabs_shifter; sh++) printf("\t");

                        //export belly
                        printf ("%s\n", node->content[0]->belly);

                    //tabs_shifting
                    for (int sh=0; sh<tabs_shifter; sh++) printf("\t");

                    //clossing taggy
                    printf("</%s>\n", node->content[0]->head);
            }
            else
            {       //tabs_shifting
                    for (int sh=0; sh<tabs_shifter; sh++) printf("\t");

                    //export atribbutes
                    for (int t=0; t<node->content_counter; t++)
                    {
                        if (t==0) printf("<%s",node->content[t]->head);
                        else printf(" %s=\"%s\"",node->content[t]->head, node->content[t]->belly);
                    }
                    printf(">\n");

                        //tabs_shifting
                        for (int sh=0; sh<tabs_shifter; sh++) printf("\t");

                        //export belly
                        printf ("%s\n", node->content[0]->belly);

                    //tabs_shifting
                    for (int sh=0; sh<tabs_shifter; sh++) printf("\t");

                    //closing taggy
                    printf("</%s>\n", node->content[0]->head);
                    return;  //PLUG
            };
        }

        //helper
        void Export_Screen_Content(struct Node* node)
        {
            tabs_shifter = 0;
            Export_Screen_Content_Tabs(node);
        }


        //stream exports
        //write to file
        char* xml_buffer;
        void memXmlBuffer(size_t size)
        {
           xml_buffer = (char*)malloc(size*sizeof(char));
           for (int i=0; i<size; i++) xml_buffer[i] = '\0';
           return;
        }

        void deleashXmlBuffer()
        {
           for(int i=0; i<strlen(xml_buffer); i++) xml_buffer[i] = '\0';
           return;
        }

        void dedicateBitZeroXmlBuffer()
        {
            xml_buffer[0] = '\0';
        }

        void Export_by_FILEFlow(FILE* flow, struct Node* node)
        {
            if (flow != NULL)
            {
               //two cases if no child and childes
               if (node->eternalCount > 0)
               {
                    //tabs_shifting
                    for (int sh=0; sh<tabs_shifter; sh++) {
                        deleashXmlBuffer();
                        xml_buffer = strcat(xml_buffer,"\t");
                        fputs(xml_buffer,flow);
                    };

                    for (int t=0; t<node->content_counter; t++)
                        {
                                deleashXmlBuffer();
                                if (t==0)
                                {
                                    xml_buffer = strcat(xml_buffer,"<");
                                    xml_buffer = strcat(xml_buffer,node->content[t]->head);
                                }
                                else
                                {
                                    xml_buffer =  strcat(xml_buffer," ");
                                    xml_buffer =  strcat(xml_buffer,node->content[t]->head);
                                    xml_buffer =  strcat(xml_buffer,"=\"");
                                    xml_buffer =  strcat(xml_buffer,node->content[t]->belly);
                                    xml_buffer =  strcat(xml_buffer,"\"");
                                }
                                fputs(xml_buffer,flow);
                        }

                     deleashXmlBuffer();
                     xml_buffer = strcat(xml_buffer, ">\n");
                     fputs(xml_buffer, flow);

                    tabs_shifter++;

                    for(int i=0; i<node->eternalCount; i++)
                            {
                                Export_by_FILEFlow(flow, node->chain[i]); //RECURSIVE STEP
                            }
                                tabs_shifter--;

                                //tabs_shifting
                                for (int sh=0; sh<tabs_shifter; sh++) {
                                    deleashXmlBuffer();
                                    xml_buffer = strcat(xml_buffer,"\t");
                                    fputs(xml_buffer,flow);
                                };

                                //export belly
                                deleashXmlBuffer();
                                xml_buffer = strcat(xml_buffer,node->content[0]->belly);
                                xml_buffer = strcat(xml_buffer,"\n");
                                fputs(xml_buffer,flow);

                                //tabs_shifting
                                for (int sh=0; sh<tabs_shifter; sh++) {
                                    deleashXmlBuffer();
                                    xml_buffer = strcat(xml_buffer,"\t");
                                    fputs(xml_buffer,flow);
                                };


                            //clossing taggy
                            deleashXmlBuffer();
                            xml_buffer = strcat(xml_buffer,"</");
                            xml_buffer = strcat(xml_buffer,node->content[0]->head);
                            xml_buffer = strcat(xml_buffer,">\n");
                            fputs(xml_buffer,flow);
               }
               else
               {
                            //tabs_shifting
                            for (int sh=0; sh<tabs_shifter; sh++) {
                                deleashXmlBuffer();
                                xml_buffer = strcat(xml_buffer,"\t");
                                fputs(xml_buffer,flow);
                            };

                            //export atribbutes
                            for (int t=0; t<node->content_counter; t++)
                            {
                                deleashXmlBuffer();
                                if (t==0)
                                {
                                    xml_buffer = strcat(xml_buffer,"<");
                                    xml_buffer = strcat(xml_buffer,node->content[t]->head);
                                }
                                else
                                {
                                    xml_buffer =  strcat(xml_buffer," ");
                                    xml_buffer =  strcat(xml_buffer,node->content[t]->head);
                                    xml_buffer =  strcat(xml_buffer,"=\"");
                                    xml_buffer =  strcat(xml_buffer,node->content[t]->belly);
                                    xml_buffer =  strcat(xml_buffer,"\"");
                                }
                                fputs(xml_buffer,flow);
                            }
                            deleashXmlBuffer();
                            xml_buffer = strcat(xml_buffer, ">\n");
                            fputs(xml_buffer, flow);

                                //tabs_shifting
                                for (int sh=0; sh<tabs_shifter; sh++) {
                                    deleashXmlBuffer();
                                    xml_buffer = strcat(xml_buffer,"\t");
                                    fputs(xml_buffer,flow);
                                };

                                //export belly
                                deleashXmlBuffer();
                                xml_buffer = strcat(xml_buffer,node->content[0]->belly);
                                xml_buffer = strcat(xml_buffer,"\n");
                                fputs(xml_buffer,flow);

                            //tabs_shifting
                            for (int sh=0; sh<tabs_shifter; sh++) {
                                deleashXmlBuffer();
                                xml_buffer = strcat(xml_buffer,"\t");
                                fputs(xml_buffer,flow);
                            };

                            //closing taggy
                            deleashXmlBuffer();
                            xml_buffer = strcat(xml_buffer,"</");
                            xml_buffer= strcat(xml_buffer,node->content[0]->head);
                            xml_buffer = strcat(xml_buffer,">\n");
                            fputs(xml_buffer,flow);

                            return;  //PLUG
                 }

            }
            else
            {
               printf("EXPORT_FLOW_ERR: is NULL pointer incoming\n");
               return;
            }
        }


        //write to file helper
        enum My_BOOL Export_By_FileName(struct Node* node, char* filepath, size_t xml_buffer_size)
        {
            enum My_BOOL rBL;
            rBL = 0;

            if(filepath != NULL && xml_buffer_size >= 2000)
            {
                   printf ("Starting export from node %x\n", node);

                   FILE* flow = fopen(filepath, "w");

                        //throw it to recursion
                        tabs_shifter = 0;
                        memXmlBuffer(xml_buffer_size);

                         /* (testing single mechanics)
                         deleashXmlBuffer();
                         xml_buffer = strcat(xml_buffer,"\tSome word\n");
                         fputs(xml_buffer, flow);

                         deleashXmlBuffer();
                         xml_buffer = strcat(xml_buffer,"\tAgain me word me please\n");
                         fputs(xml_buffer, flow);

                         deleashXmlBuffer();
                         for (int i=0; i<5; i++) { xml_buffer = strcat(xml_buffer,"O'Cmmon\n"); }
                         fputs(xml_buffer, flow);

                         deleashXmlBuffer();
                         xml_buffer = strcat(xml_buffer,"\tJust i am so sexy\n");
                         fputs(xml_buffer, flow);
                         */

                        Export_by_FILEFlow(flow, node);

                        //loose my memory
                        free(xml_buffer);

                   fclose(flow);

                   rBL = 1;

                /*
                past file asqpurpose
                FILE* flow = fopen(filepath,"r");
                if (flow == NULL)
                {


                }
                else
                {
                    printf("EXPORT_BY_FNAME_REPORT: sorry, path incorrect or file is exist!!! try rename or share by explorer!\n");
                    rBL = 0;
                }
                */
            }
            else
            {
            printf("EXPORT_BY_FNAME_ERR: is NULL pointer incoming or buffer size less than 2000\n");
            rBL = 0;
            }
        return rBL;
        }

//******************************reading a tree
//READING A TREE!   STOPPED CRASH IN BUILDING TREE!!!

struct Node* Build_From_File (struct Node* current, char* filepath, size_t strbuffersize)
{
  FILE* flow = fopen(filepath, "r");

  //create a buffer to reading
  memXmlBuffer(strbuffersize);

  int polynomial = 0;

  if(filepath != NULL && flow != NULL)
  {
        printf("Can read current file is exist!\n");

                //preliminary stage
                //couters
                int level = 0;
                int counter = 0;
                    char attr[2000];
                    char rootBody[2000];

                //working chars
                char in_mind = '>';
                char read;

                //childer
                struct Node* childer;

                //construct root
                current = Node_Cnstr_Empty(current, 112, "Root", "Root--Body");

        //building a tree inside cycle
        while ((read = fgetc(flow)) != EOF)
        {
              //parsing ...
              if(in_mind == '>' && read == '<')
              {
                  in_mind = read;

                  xml_buffer[counter] = '\0';

                    //FILL TREE adopted new body
                    if (level>0) current = Node_Adobt_Body(current, xml_buffer);

                    if (strlen(xml_buffer)>0) printf("body %s\n", xml_buffer); //cause in xml_buffer acumulated or nothing
                   counter = 0;
                  xml_buffer[0] = '\0';
              }
              else if (in_mind == '<' && (read == ' ' || read == '>'))
              {
                  in_mind = read;

                  //up level
                  level++;

                  xml_buffer[counter] = '\0';

                       //FILL TREE addition of new node! step downyard!
                       if (level > 0)
                       {
                          childer = Node_Cnstr_Empty(childer, level, xml_buffer, "0");
                          Node_Attach_Child(current, childer);
                          current = childer;
                       }

                    printf("tag %s\n", xml_buffer); //cause in xml_buffer acumulated tag
                   counter = 0;
                  xml_buffer[0] = '\0';
              }
              else if(read == '=' && (in_mind == ' ' || in_mind == '\"'))
              {
                 in_mind = read;

                 xml_buffer[counter] = '\0';

                       //FILL TREE
                       strcpy(attr, xml_buffer);

                   printf("attr %s\n", xml_buffer); //cause in xml_buffer acumulated attribute
                  counter = 0;
                 xml_buffer[0] = '\0';
              }
              else if (in_mind == '=' && read == '\"')
              {
                  in_mind = read;
                  //this case where we want defeat quotes
                  printf("quotes(=)(\")\n");
                  counter = 0;
                xml_buffer[0] = '\0';
              }
              else if(in_mind == '\"' && read == '\"')
              {
                in_mind = read;
                  xml_buffer[counter] = '\0';
                  printf("atr_trunk %s\n", xml_buffer); //this case we find our attribute belly

                        //FILL TREE atribute insertion to current
                        current = Node_Add_Atribute(current, attr, xml_buffer);

                  counter = 0;
                xml_buffer[0] = '\0';
              }
              else if (in_mind == '\"' && read == '>')
              {
                //this case where we want defeat quotes
                printf("quotes(\")(>)\n");
                in_mind = read;
                   counter = 0;
                xml_buffer[0] = '\0';
              }
              else if (in_mind == '<' && read == '/')
              {
                in_mind = read;
                  xml_buffer[counter] = '\0';
                  printf("%s closed current level %d\n", xml_buffer, level);
                  counter = 0;
                xml_buffer[0] = '\0';

                //down level
                level--;

                //FILL TREE step upyard
                current = current->federal;
              }
              else if(in_mind == '/' && read == '>')
              {
                in_mind = read;
                  xml_buffer[counter] = '\0';
                  printf("closing tag %s\n", xml_buffer);
                  counter = 0;
                xml_buffer[0] = '\0';
              }
              //(32 space 9 tabs 10 enter)
              else if (read != 32 && read != 9 && read != 10)
              {
                xml_buffer[counter++] = read;
              }
              else {};

        }//end of while
        fclose(flow);
  }
  else
  {
     printf("Build_From_File_ERR: Sorry, but file is not exist!\n");
  }

  free(xml_buffer);

  //current = current->chain[0]->federal;

  return current;
}




int main()
{
    printf("Hello tree! NULLNODE %x \n", &nullNode);

    printf ("Locale out %s\n", setlocale(LC_ALL, "en_US.UTF-8"));


    printf ("root...\n");
        struct Node* root;
        //root = Node_Cnstr_Empty(root, 112, "my_root", "my_root_value");


        /*  Test Adobt resizer!!!
        root = Node_Adobt_Body(root, "very long long long long longer string");
        root = Node_Adobt_Body(root, "very long long long long longer very long long long long longer hhhh string");
        printf("roots sizeH %d  %s, sizeB %d  %s \n", root->content[0]->head_length
                                                    , root->content[0]->head
                                                    , root->content[0]->belly_length
                                                    , root->content[0]->belly);
        */


    //BUILDING HERE    !!!!
    root = Build_From_File(root, "./margarita.dat", 2000);

    Export_Screen_Content(root);
    printf("TESTED %x  null %x\n", root, &nullNode);
    Export_By_FileName(root, "./quantro.dat", 2000);
    Node_Destr_Curr(root);


     /* Test Creation of new tree!

                    struct Node* current; //add his freand currentinio
                          current = root;

                     for (int i=0; i<3; i++)  current =  Node_Add_Atribute(current,"rootribute_head", "rootribute_body");




                    //comon childer pointer
                    struct Node* chld;


    printf("delim...\n");
    struct Node* delim;
    delim = Node_Cnstr_Empty(delim, 88, "delim", "delim_value");
    for (int i=0; i<3; i++) delim = Node_Add_Atribute(delim, "delim n_attrbute", "delim_atrr_belly");

            for (int i=97; i<102; i++)
            {
                char f[2] = {(char)i,'\0'};
                chld = Node_Cnstr_Empty(chld, 80+i, "delim_child", f);
                Node_Attach_Child(delim,chld);
            }

    printf("spicer...\n");
    //spicing test
    struct Node* spicer;
    spicer = Node_Cnstr_Empty(spicer, 666, "SpicerTest", "SpicerTest_value");

                    int x=110;
                    while (x<113)
                    {
                        if (x==111)
                        {
                                Node_Attach_Child(spicer,delim);
                                x++;
                        }
                        else
                        {
                                char f[2] = { (char)x,'\0'};
                                chld = Node_Cnstr_Empty(chld, 20+(x++), "Spicer_child", f);
                                    Node_Attach_Child(spicer,chld);
                        }
                    }

   printf("childer...\n");
   for (int i=102; i<107; i++)
   {
            if (i == 105)
            {
                Node_Attach_Child(current, spicer);
            }
            else
            {
                char f[2] = { (char)i,'\0'};
                chld = Node_Cnstr_Empty(chld, i, "first_child", f);
                Node_Attach_Child(current, chld);
            }
   }



      //Export_Screen_Int(current);
      printf("\n\nHellow beauty!\n\n");
      Export_Screen_Content(root);
      printf ("Export return %d\n", Export_By_FileName("./margarita.dat", current, 2000));

      Node_Destr_Curr(root);
      */



                                /*
                                //Try to step throw the tree
                                printf("\n\nproceed tests...\n");

                                current = proceed_Down(current, 0);

                                chld = Node_Cnstr_Empty(chld, 3, 20);
                                Node_Attach_Child(current, chld, 0);

                                chld = Node_Cnstr_Empty(chld, 0, 21);
                                Node_Attach_Child(current, chld, 1);

                                chld = Node_Cnstr_Empty(chld, 0, 22);
                                Node_Attach_Child(current, chld, 2);

                                chld = Node_Cnstr_Empty(chld, 3, 23);
                                Node_Attach_Child(current, chld, 3);

                                chld = Node_Cnstr_Empty(chld, 0, 28);
                                Node_Attach_Child(current, chld, 8);

                                current = proceed_Down(current, 0);

                                chld = Node_Cnstr_Empty(chld, 0, 30);
                                Node_Attach_Child(current, chld, 0);

                                chld = Node_Cnstr_Empty(chld, 0, 31);
                                Node_Attach_Child(current, chld, 1);

                                chld = Node_Cnstr_Empty(chld, 0, 32);
                                Node_Attach_Child(current, chld, 2);

                                current = proceed_Up(current);
                                current = proceed_Down(current, 3);

                                chld = Node_Cnstr_Empty(chld, 0, 330);
                                Node_Attach_Child(current, chld, 0);

                                chld = Node_Cnstr_Empty(chld, 0, 331);
                                Node_Attach_Child(current, chld, 1);

                                chld = Node_Cnstr_Empty(chld, 2, 332);
                                Node_Attach_Child(current, chld, 2);

                                showCurrentNode(current);
                                showCurrentChain(current);

                                */
   return 0;
}
