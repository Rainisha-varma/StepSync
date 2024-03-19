#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define SIZE 100
#define WEEK 7
#define MAX_LINE_LENGTH 200
#define MAX_INDIVIDUALS 100 // Assuming a maximum number of individuals in the file
#define MAX_IND_GRP 5

typedef enum{FAILURE,SUCCESS} status_code;


typedef struct Individual
{
   int ind_id;
   char name[SIZE];
   int age;
   int daily_step_goal;
   int step_count[WEEK];
   int group_id;
   struct Individual*next;
} individual;

typedef struct Member
{
   int mem_id;
   individual* mem_ptr;
   struct Member*next;
}member;   

typedef struct Group
{
   int grp_id;
   char grp_name[SIZE];
   member*grp_mem_ptr;
   int weekly_group_goal;
   struct Group*next;
}group;

static int individual_id=0;
static int group_id=0;


individual *ind_lptr=NULL;
group *grp_lptr=NULL;

status_code extract_ind_csv() 
{
    FILE *file = fopen("try.csv", "r");
    if (file == NULL) 
    {
        printf("INVALID FILE");
        exit(EXIT_FAILURE);
    }

    status_code sc = SUCCESS;
    individual *ptr=NULL;
    char line[MAX_LINE_LENGTH];

    while (fgets(line, sizeof(line), file) != NULL) 
    {
        individual *nptr = (individual*)malloc(sizeof(individual));
        if (nptr == NULL) 
        {
            // Memory allocation failed
            sc = FAILURE;
        }

        sscanf(line, "%d,%99[^,],%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n",
               &nptr->ind_id,
               nptr->name,
               &nptr->age,
               &nptr->group_id,
               &nptr->daily_step_goal,
               &nptr->step_count[0],
               &nptr->step_count[1],
               &nptr->step_count[2],
               &nptr->step_count[3],
               &nptr->step_count[4],
               &nptr->step_count[5],
               &nptr->step_count[6]);

        nptr->next = NULL;
        individual_id=nptr->ind_id;

        if (ind_lptr == NULL) {
        
            ind_lptr = nptr;
            ptr = ind_lptr;
        } 
        else 
        {
            ptr->next=nptr;
            ptr =nptr;
        }
    }
    fclose(file);
    return sc;
}

individual* find_group_mem(int id)
{
   individual* ret_mem_ptr=NULL;
   if(id!=0)
   {
      ret_mem_ptr=ind_lptr;
      while(ret_mem_ptr!=NULL && id!=ret_mem_ptr->ind_id)
      {
         ret_mem_ptr=ret_mem_ptr->next;
      }
   }
   return ret_mem_ptr;
}         


status_code extract_grp_csv() 
{
    FILE *file = fopen("grp.csv", "r");
    if (file == NULL) 
    {
        printf("INVALID FILE");
        exit(EXIT_FAILURE);
    }

    status_code sc = SUCCESS;
    individual *ptr=NULL;
    char line[MAX_LINE_LENGTH];
    int temp_id[MAX_IND_GRP];

    while (fgets(line, sizeof(line), file) != NULL) 
    {
        group *grp_nptr = (group*)malloc(sizeof(group));
        if (grp_nptr == NULL) 
        {
            // Memory allocation failed
            sc = FAILURE;
        }

        sscanf(line, "%d,%99[^,],%d,%d,%d,%d,%d,%d\n",
               &grp_nptr->grp_id,
               grp_nptr->grp_name,
               &temp_id[0],
               &temp_id[1],
               &temp_id[2],
               &temp_id[3],
               &temp_id[4],
               &grp_nptr->weekly_group_goal
               );
         grp_nptr->grp_mem_ptr=NULL;
         group_id=grp_nptr->grp_id;
         for(int i=0;i< MAX_IND_GRP;i++)
         {
               if(temp_id[i]!=0)
               {
                   ptr=find_group_mem(temp_id[i]);
                   member *mem_nptr=(member *)malloc(sizeof(member));
                   mem_nptr->mem_id=temp_id[i];
                   mem_nptr->mem_ptr=ptr;
                   mem_nptr->next=NULL;
               if(grp_nptr->grp_mem_ptr==NULL)
               {
                    grp_nptr->grp_mem_ptr=mem_nptr;
               }
               else
               {
                    member *temp_mem=grp_nptr->grp_mem_ptr;
                    while(temp_mem->next!=NULL)
                    {
                        temp_mem=temp_mem->next;
                    }
                    temp_mem->next=mem_nptr;
               }
               }
          }
           
          if(grp_lptr==NULL)
          {
              grp_lptr= grp_nptr;
          }
          else
          {
              group *temp_grp=grp_lptr;
              while(temp_grp->next!=NULL)
              {
                  temp_grp=temp_grp->next;
              }
              temp_grp->next=grp_nptr;
          }
        
    }
    fclose(file);
    return sc;
}

individual*create_individual(char*n,int a,int dsg)
{
  individual*newMember;
  newMember=(individual*)malloc(sizeof(individual));
  newMember->ind_id=++individual_id;
  strcpy(newMember->name,n);
  newMember->age=a;
  newMember->daily_step_goal=dsg;
  for(int i=0;i<WEEK;i++)
  {
     newMember->step_count[i]=0;
  }
  newMember->group_id=0;   
  newMember->next=NULL;
  return newMember;
}

status_code add_person()
{
    status_code sc=SUCCESS;
    individual *newMember;
    char name[SIZE];
    int age,dsg;
    getchar();
    printf("Enter the name: ");
    fgets(name,sizeof(name),stdin);
    name[strcspn(name, "\n")] = '\0';
    printf("Enter the age: ");
    scanf("%d",&age);
    printf("Enter the daily step goal: ");
    scanf("%d",&dsg);
    newMember=create_individual(name,age,dsg);
    if(newMember==NULL)
    {
       sc=FAILURE;
    }
    else
    {
       if(ind_lptr==NULL)
       {
          ind_lptr=newMember;
       }
       else
       {
          individual *ptr=ind_lptr;
          while(ptr->next!=NULL)
          {
             ptr=ptr->next;
          }
          ptr->next=newMember;
       }
        
    }
    return sc;
}             

status_code create_group()
{
    status_code sc=SUCCESS;
    char ans='y';
    char name[SIZE];
    int id,wgg;
    member *mem_nptr=NULL;
    individual *temp;
    getchar();
    printf("ENTER GROUP NAME:");
    fgets(name,SIZE,stdin);
    name[strcspn(name,"\n")]='\0';
    printf("ENTER WEEKLY GROUP GOAL: ");
    scanf("%d",&wgg);
    int count=0;
    group *grp=(group *)malloc(sizeof(group));
    grp->next=NULL;
    strcpy(grp->grp_name,name);
    grp->grp_id=++group_id;
    grp->weekly_group_goal=wgg;
    grp->grp_mem_ptr=NULL;
    while(count<5 && ans=='y')
    {
       printf("Enter id of member :\n");
       scanf("%d",&id);
       getchar();
       temp=find_group_mem(id);
       if(temp==NULL)
       {
          printf("THE ENTERED INDIVIDUAL ID IS INVALID!!\n");
       }
       else 
       {
          //printf("ELSE\n");
          if(temp->group_id!=0)
          {
             printf("THIS PARTICULAR INDIVIDUAL IS IN ANOTHER GROUP\n");
          }
          else
          {
             count++;
             mem_nptr=(member *)malloc(sizeof(member));
             mem_nptr->mem_id=id;
             mem_nptr->mem_ptr=temp;
             mem_nptr->mem_ptr->group_id=grp->grp_id;
             mem_nptr->next=NULL;
             if(grp->grp_mem_ptr==NULL)
             {
                    grp->grp_mem_ptr=mem_nptr;
             }
             else
             {
                    member *temp_mem=grp->grp_mem_ptr;
                    while(temp_mem->next!=NULL)
                    {
                        temp_mem=temp_mem->next;
                    }
                    temp_mem->next=mem_nptr;
             }
           }
        }
           printf("DO YOU WANT TO ADD MORE:(y/n)\n");
           scanf("%c",&ans);
      }
      if(count==5 && ans=='y')
      {
        printf("GROUP MEMBER LIMIT EXCEEDED!!\n");
      }
      if(count==0)
      {
         group_id--;
         //printf("%p\n",mem_nptr);
         //free(mem_nptr);
         free(grp);
         sc=FAILURE;
      }
      else
      {
         
         if(grp_lptr==NULL)
          {
              grp_lptr= grp;
          }
          else
          {
              group *temp_grp=grp_lptr;
              while(temp_grp->next!=NULL)
              {
                  temp_grp=temp_grp->next;
              }
              temp_grp->next=grp;
          }
      }
      return sc;
}

status_code delete_group(int id)
{
   //printf("DELETE GRP START\n");
   status_code sc=SUCCESS;
   group *prev,*temp=grp_lptr;
   member *temp_mem,*next_mem;
   prev=NULL;
   while(temp!=NULL && temp->grp_id!=id)
   {
        prev=temp;
        temp=temp->next;
   }
   if(temp==NULL)
   {
        sc=FAILURE;
        printf("GROUP ID IS INVALID!!\n");
   }
   else
   {
       if(prev==NULL)
       {
           grp_lptr=grp_lptr->next;
           //printf("Delete grp prev==NULL\n");
       }
       else
       {
           prev->next=temp->next;
           //printf("Delete grp prev not NULL\n");
       }
       temp_mem=temp->grp_mem_ptr;
       while(temp_mem!=NULL)
       {
           //printf("WHILE\n");
           if(temp_mem->mem_ptr!=NULL)
           {

           temp_mem->mem_ptr->group_id=0;
           }
           next_mem=temp_mem->next;
           
           //printf("Before free\n");
           free(temp_mem);
           temp_mem=next_mem;
           //printf("after free\n");
           
       }
       

       free(temp);
       //printf("Freed group\n");
    }
    //printf("DELETE GROUP END\n");
    return sc;
}

status_code delete_individual(int id)
{
    status_code sc=SUCCESS;
    int gp_id;
    individual *prev,*temp=ind_lptr;
    group *curr_grp=grp_lptr;
    prev=NULL;
    while(temp!=NULL && temp->ind_id!=id)
    {
         prev=temp;
         temp=temp->next;
    }
    if(temp==NULL)
    {
       sc=FAILURE;
       printf("ID DOESN'T EXIST\n");
    }
    else
    {
       if(prev==NULL)
       {
           ind_lptr=ind_lptr->next;
       }
       else
       {
           prev->next=temp->next;
       }
       gp_id=temp->group_id;
       if(gp_id!=0)
       {
           while(curr_grp!=NULL && curr_grp->grp_id!=gp_id)
          {
              curr_grp=curr_grp->next;
          }
          member *prev_mem,*curr_mem=curr_grp->grp_mem_ptr;
          prev_mem=NULL;
          while(curr_mem!=NULL && curr_mem->mem_id!=temp->ind_id)
          {
              prev_mem=curr_mem;
              curr_mem=curr_mem->next;
          }
          if(prev_mem==NULL)
          {
               curr_grp->grp_mem_ptr=curr_grp->grp_mem_ptr->next;
          }
          else
          {
                prev_mem->next=curr_mem->next;
          }
          free(curr_mem);
          if(curr_grp->grp_mem_ptr==NULL)
          {
                sc=delete_group(curr_grp->grp_id);
          }
      }
       free(temp);
    }
    return sc;
}           
             
status_code merge_groups(int grp_id1, int grp_id2) 
{
    status_code sc = SUCCESS;
    group *grp_ptr1 = NULL, *grp_ptr2 = NULL;
    group *temp = grp_lptr;
    char name[SIZE];
    int wgg;

    // Find the groups with the given IDs
    while (temp != NULL) 
    {
        if (temp->grp_id == grp_id1) 
        {
            grp_ptr1 = temp;
        } 
        else if (temp->grp_id == grp_id2) 
        {
           grp_ptr2 = temp;
        }
        temp = temp->next;
    }

    // Check if both groups were found
    if (grp_ptr1 == NULL || grp_ptr2 == NULL) 
    {
        sc = FAILURE;
        printf("INVALID!!");
    } 
    else 
    {
        // Check if the total number of members in the merged group exceeds the limit
        int total_members = 0;
        member *temp_mem = grp_ptr1->grp_mem_ptr;
        while (temp_mem != NULL) 
        {
            total_members++;
            temp_mem = temp_mem->next;
        }
        temp_mem = grp_ptr2->grp_mem_ptr;
        while (temp_mem != NULL) 
        {
            total_members++;
            temp_mem = temp_mem->next;
        }

        if (total_members > MAX_IND_GRP) 
        {
            sc = FAILURE;
            printf("EXCEEDING GROUP MEMBER LIMIT!!\n");
        } 
        else 
        {
            getchar();
            printf("ENTER NEW GROUP NAME:\n");
            fgets(name, SIZE, stdin);
            name[strcspn(name, "\n")] = '\0';
            printf("ENTER WEEKLY GROUP GOAL: \n");
            scanf("%d", &wgg);

            // Create a new group and copy members
            group *grp = (group *)malloc(sizeof(group));
            if (grp == NULL) 
            {
                // Handle memory allocation failure
                printf("Memory allocation failed\n");
                sc = FAILURE;
            } 
            else 
            {
                grp->next = NULL;
                strcpy(grp->grp_name, name);
                grp->grp_id = ++group_id;
                grp->weekly_group_goal = wgg;
                grp->grp_mem_ptr = NULL;

                // Copy members from grp_ptr1
                temp_mem = grp_ptr1->grp_mem_ptr;
                while (temp_mem != NULL) 
                {
                    member *mem = (member *)malloc(sizeof(member));
                    if (mem == NULL) 
                    {
                        // Handle memory allocation failure
                        printf("Memory allocation failed\n");
                        sc = FAILURE;
                        //break; // Exit loop on failure
                    }
                    mem->mem_id = temp_mem->mem_id;
                    mem->mem_ptr = temp_mem->mem_ptr;
                    mem->next = NULL;

                    if (grp->grp_mem_ptr == NULL) 
                    {
                        grp->grp_mem_ptr = mem;
                    } 
                    else 
                    {
                        member *mem_last = grp->grp_mem_ptr;
                        while (mem_last->next != NULL) 
                        {
                            mem_last = mem_last->next;
                        }
                        mem_last->next = mem;
                    }
                    temp_mem = temp_mem->next;
                }

                // Copy members from grp_ptr2
                if (sc == SUCCESS) 
                {
                    temp_mem = grp_ptr2->grp_mem_ptr;
                    while (temp_mem != NULL) 
                    {
                        member *mem = (member *)malloc(sizeof(member));
                        if (mem == NULL) 
                        {
                    
                            // Handle memory allocation failure
                            printf("Memory allocation failed\n");
                            sc = FAILURE;
                            break; // Exit loop on failure
                        }
                        mem->mem_id = temp_mem->mem_id;
                        mem->mem_ptr = temp_mem->mem_ptr;
                        mem->next = NULL;

                        if (grp->grp_mem_ptr == NULL) 
                        {
                            grp->grp_mem_ptr = mem;
                        } 
                        else 
                        {
                            member *mem_last = grp->grp_mem_ptr;
                            while (mem_last->next != NULL) 
                            {
                                mem_last = mem_last->next;
                            }
                            mem_last->next = mem;
                        }
                        temp_mem = temp_mem->next;
                    }
                }

                // Delete original groups if all memory allocations were successful
                if (sc == SUCCESS) 
                {
                    sc = delete_group(grp_id1);
                    if (sc != SUCCESS) 
                    {
                        printf("Failed to delete group %d\n", grp_id1);
                        // Handle failure
                    }

                    sc = delete_group(grp_id2);
                    if (sc != SUCCESS) 
                    {
                        printf("Failed to delete group %d\n", grp_id2);
                        // Handle failure
                    }

                    // Update group IDs for members in the merged group
                    temp_mem = grp->grp_mem_ptr;
                    while (temp_mem != NULL) 
                    {
                        temp_mem->mem_ptr->group_id = group_id;
                        temp_mem = temp_mem->next;
                    }

                    // Add the new group to the list
                    if (grp_lptr == NULL) 
                    {
                        grp_lptr = grp;
                    } 
                    else 
                    {
                        temp = grp_lptr;
                        while (temp->next != NULL) 
                        {
                            temp = temp->next;
                        }
                        temp->next = grp;
                    }
                }
            }
        }
    }

    return sc;
}

int calculate_total_steps(member *grp_mem_ptr) {
    int total_steps = 0;
    while (grp_mem_ptr != NULL) {
        for (int i = 0; i < WEEK; i++) {
            total_steps += grp_mem_ptr->mem_ptr->step_count[i];
        }
        grp_mem_ptr = grp_mem_ptr->next;
    }
    return total_steps;
}

group* merge(group *list1, group *list2) {
    if (list1 == NULL) return list2;
    if (list2 == NULL) return list1;

    group *result;
    if (calculate_total_steps(list1->grp_mem_ptr) >= calculate_total_steps(list2->grp_mem_ptr)) {
        result = list1;
        result->next = merge(list1->next, list2);
    } else {
        result = list2;
        result->next = merge(list1, list2->next);
    }
    return result;
}

void split(group *head, group **front, group **back) {
    if (head == NULL || head->next == NULL) {
        *front = head;
        *back = NULL;
        return;
    }

    group *slow = head;
    group *fast = head->next;

    while (fast != NULL) {
        fast = fast->next;
        if (fast != NULL) {
            slow = slow->next;
            fast = fast->next;
        }
    }

    *front = head;
    *back = slow->next;
    slow->next = NULL;
}

group* merge_sort(group *head) {
    if (head == NULL || head->next == NULL) {
        return head;
    }

    group *front;
    group *back;

    split(head, &front, &back);

    front = merge_sort(front);
    back = merge_sort(back);

    return merge(front, back);
}

status_code generate_leader_board() 
{
    // Make a copy of the original group list
    status_code sc=SUCCESS;
    group *copy_grp_lptr = NULL;
    group *temp = grp_lptr;
    group *last = NULL;

    while (temp != NULL) 
    {
        group *new_group = (group *)malloc(sizeof(group));
        if (new_group == NULL) 
        {
            sc=FAILURE; // Memory allocation failed
        }

        else
        {
           memcpy(new_group, temp, sizeof(group));
           new_group->next = NULL;

           if (copy_grp_lptr == NULL) 
          {
             copy_grp_lptr = new_group;
          } 
          else 
          {
             last->next = new_group;
          }

          last = new_group;
          temp = temp->next;
        }  
    }

    // Sort the copied group list based on total steps
    copy_grp_lptr = merge_sort(copy_grp_lptr);

    // Display the leader board
    printf("Leader Board:\n");
    printf("Group ID\tGroup Name\t\tTotal Steps\n");

    group *temp_grp = copy_grp_lptr;
    while (temp_grp != NULL) 
    {
        printf("%d\t\t%s\t\t%d\n", temp_grp->grp_id, temp_grp->grp_name, calculate_total_steps(temp_grp->grp_mem_ptr));
        temp_grp = temp_grp->next;
    }

    // Free the memory allocated for the copied list
    while (copy_grp_lptr != NULL) 
    {
        group *temp = copy_grp_lptr;
        copy_grp_lptr = copy_grp_lptr->next;
        free(temp);
    }

    return sc;
}

status_code check_group_achievement(int id)
{
   status_code sc=SUCCESS;
    group *grp_ptr1=NULL;
    member *temp_mem;
    grp_ptr1=grp_lptr;
    while(grp_ptr1!=NULL && grp_ptr1->grp_id!=id)
    {
       grp_ptr1=grp_ptr1->next;
    }
    if(grp_ptr1==NULL)
    {
       printf("INVALID GROUP ID\n");
    }
    else
    {
        //calculate_total_steps(member *grp_mem_ptr)
        temp_mem=grp_ptr1->grp_mem_ptr;
        int total_count=0;     
        //while(temp_mem!=NULL)
        //{
            total_count=calculate_total_steps(temp_mem);
            //temp_mem=temp_mem->next;
        // }
         //printf("%d\n",total_count);
         if(total_count>=grp_ptr1->weekly_group_goal)
         {
            printf("Congratulations!! Weekly step goal achieved!! Keep it up!!\n");
         }
         else
         {
            printf("Weekly step goal  not achieved!! No worries you can do it!!\n");
         }
     }
     return sc;
}    


individual* get_top_3() 
{
    individual *temp_ind = ind_lptr, *top = NULL, *ind = NULL, *last = NULL, *copy = NULL;
    int weekly_steps = 0;
    int max_steps = 0, flag = 1;

    // Find the individual with the highest weekly steps
    while (temp_ind != NULL) 
    {
        weekly_steps = 0;
        flag = 1;

        // Calculate weekly steps
        for (int i = 0; i < WEEK && flag; i++) 
        {
            if (temp_ind->step_count[i] < temp_ind->daily_step_goal) 
            {
                flag = 0;
            } 
            else 
            {
                weekly_steps += temp_ind->step_count[i];
            }
        }

        // Update the highest weekly steps and corresponding ID
        if (weekly_steps > max_steps && flag) 
        {
            max_steps = weekly_steps;
            //id = temp_ind->ind_id;
            ind=temp_ind;
        }

        temp_ind = temp_ind->next;
    }

    // If no individual meets the criteria, print a message
    if (ind == NULL) 
    {
        printf("NO ONE COMPLETED THEIR DAILY GOAL!!\n");
    } 
    else
    {
        // Find the individual with the given ID
        //ind = find_group_mem(id);

        // Copy the first individual to 'top'
        top = (individual*)malloc(sizeof(individual));
        memcpy(top, ind, sizeof(individual));
        top->next = NULL;
        last = top;

        // Reset max_steps and id for next iteration
        max_steps = 0;
        ind=NULL;

        // Find the next two individuals
        temp_ind = ind_lptr;
        while (temp_ind != NULL) 
        {
            if (top->ind_id != temp_ind->ind_id) 
            {
                weekly_steps = 0;
                flag = 1;

                // Calculate weekly steps
                for (int i = 0; i < WEEK && flag; i++) 
                {
                    if (temp_ind->step_count[i] < temp_ind->daily_step_goal) 
                    {
                        flag = 0;
                    } 
                    else 
                    {
                        weekly_steps += temp_ind->step_count[i];
                    }
                }

                // Update the highest weekly steps and corresponding ID
                if (weekly_steps > max_steps && flag) 
                {
                    max_steps = weekly_steps;
                    ind=temp_ind;
                    //id = temp_ind->ind_id;
                }
            }

            temp_ind = temp_ind->next;
        }

        // If a valid ID is found, copy the individual to 'copy'
        if (ind != NULL) 
        {
            //ind = find_group_mem(id);
            copy = (individual*)malloc(sizeof(individual));
            memcpy(copy, ind, sizeof(individual));

            // Add 'copy' to the list
            last->next = copy;
            last = last->next;
            last->next = NULL;

            // Reset max_steps and id for the next iteration
            max_steps = 0;
            ind=NULL;
            //id = 0;

            // Find the third individual
            temp_ind = ind_lptr;
            while (temp_ind != NULL) 
            {
                if (top->ind_id != temp_ind->ind_id && last->ind_id != temp_ind->ind_id) 
                {
                    weekly_steps = 0;
                    flag = 1;

                    // Calculate weekly steps
                    for (int i = 0; i < WEEK && flag; i++) 
                    {
                        if (temp_ind->step_count[i] < temp_ind->daily_step_goal) 
                        {
                            flag = 0;
                        }
                        else 
                         {
                            weekly_steps += temp_ind->step_count[i];
                         }
                    }

                    // Update the highest weekly steps and corresponding ID
                    if (weekly_steps > max_steps && flag) 
                    {
                        max_steps = weekly_steps;
                        ind=temp_ind;
                        //id = temp_ind->ind_id;
                    }
                }

                temp_ind = temp_ind->next;
            }

            // If a valid ID is found, copy the individual to 'copy'
            if (ind != NULL) 
            {
                //ind = find_group_mem(id);
                copy = (individual*)malloc(sizeof(individual));
                memcpy(copy, ind, sizeof(individual));

                // Add 'copy' to the list
                last->next = copy;
                last = last->next;
                last->next = NULL;
            }
        }
    }

    return top;
}

void print_top_3()
{
   individual *top=NULL;
   top=get_top_3();
   printf("The top 3 are: \n");
   printf("ID\tName\n");
   while(top!=NULL)
   {
      printf("%d\t%s\n",top->ind_id,top->name);
      top=top->next;
   }
}      



status_code check_individual_rewards(int id)
{
   status_code sc=SUCCESS;
   int score=0;
   individual *ind_info=NULL;
   ind_info=find_group_mem(id);
   if(ind_info==NULL)
   {
      printf("Invalid ID\n");
      sc=FAILURE;
   }   
   else
   {
      individual *top_3=NULL;
      top_3=get_top_3();
      int c=1;
      while(top_3!=NULL&&top_3->ind_id!=id)
      {
         c++;
         top_3=top_3->next;
      }   
      if(top_3!=NULL)
      {
         if(c==1)
            score=100;
         else if(c==2)
            score=75;
         else if(c==3)
            score=50;
      }
      printf("Individual Score is: %d\n",score);
   }
   return sc;
}  

status_code suggest_goal_update(int id)
{
   status_code sc=SUCCESS;
   individual *ind_info=NULL;
   ind_info=find_group_mem(id);
   if(ind_info==NULL)
   {
      printf("Invalid ID\n");
      sc=FAILURE;
   }   
   else
   {
      individual *top_3=NULL;
      top_3=get_top_3();
      
      int total=0;
      while(top_3!=NULL&&top_3->ind_id!=id)
      {
         for(int i=0;i<WEEK;i++)
         {  
             total+=top_3->step_count[i];
         }    
         top_3=top_3->next;
      }   
      if(top_3!=NULL&&top_3->ind_id==id)
      {
         printf("You are already in top 3!!");
      }
      else
      {
         int mean=total/21; //three individuals and 7 days therefore 21
        
         char ans;
         getchar();
         printf("New goal should be: %d\n",mean);
         printf("Want to update the daily step goal?[y/n]");
         scanf("%c",&ans);
         if(ans=='y')
         {
            ind_info->daily_step_goal=mean;
            printf("Goal Updated :)\n");
         }
       }
   }
   return sc;
}                            

status_code display_group_info(int id)
{
   status_code sc=SUCCESS;
   group *grp=grp_lptr;
   while(grp!=NULL && grp->grp_id!=id)
   {
        grp=grp->next;
   }
   if(grp==NULL)
   {
        sc=FAILURE;
        printf("GROUP ID IS INVALID!!\n");
   }
   else
   {
      group*temp=grp_lptr;
      group *copy_grp_lptr = NULL;
      group *last = NULL;
      while (temp != NULL) 
     {
        group *new_group = (group *)malloc(sizeof(group));
        if (new_group == NULL) 
        {
            sc=FAILURE; // Memory allocation failed
        }

        else
        {
           memcpy(new_group, temp, sizeof(group));
           new_group->next = NULL;

           if (copy_grp_lptr == NULL) 
          {
             copy_grp_lptr = new_group;
          } 
          else 
          {
             last->next = new_group;
          }

          last = new_group;
          temp = temp->next;
        }  
     }
     // Sort the copied group list based on total steps
    copy_grp_lptr = merge_sort(copy_grp_lptr);
    printf("GROUP INFORMATION: \n");
    printf("GROUP ID: %d\n",grp->grp_id);
    printf("GROUP NAME: %s\n",grp->grp_name);
    member *mem=grp->grp_mem_ptr;
    while(mem!=NULL)
    {
       printf("MEMBER ID: %d\tMEMBER NAME: %s\n",mem->mem_id,mem->mem_ptr->name);
       mem=mem->next;
    }
    int rank=1;
    while(copy_grp_lptr!=NULL && copy_grp_lptr->grp_id!=grp->grp_id)
    {
           rank++;
           copy_grp_lptr=copy_grp_lptr->next;
    }
    printf("RANK : %d\n",rank);
    // Free the memory allocated for the copied list
    while (copy_grp_lptr != NULL) 
    {
        group *temp = copy_grp_lptr;
        copy_grp_lptr = copy_grp_lptr->next;
        free(temp);
    }
    
    }
return sc;
}             

void insert_ind_file()
{
   FILE *file = fopen("try.csv", "w");
   if (file == NULL) 
   {
        perror("Error opening file");
        exit(1);
   }
   individual *temp=ind_lptr;
   while(temp!=NULL)
   {
      fprintf(file, "%d,%s,%d,%d,%d,", temp->ind_id, temp->name, temp->age,temp->group_id,temp->daily_step_goal);
      for (int i = 0; i < WEEK; i++) 
      {
        fprintf(file, "%d", temp->step_count[i]);
        if (i < 6) fprintf(file, ",");
      }
     fprintf(file, "\n");
     temp=temp->next;
  }
}   

void insert_grp_file()
{
   FILE *file = fopen("grp.csv", "w");
   if (file == NULL) 
   {
        perror("Error opening file");
        exit(1);
   }
   group *temp=grp_lptr;
   member *ind=NULL;
   while(temp!=NULL)
   {
      ind=temp->grp_mem_ptr;
      fprintf(file, "%d,%s,", temp->grp_id, temp->grp_name);
      for (int i = 0; i < 5; i++) 
      {
        int id=0;
        if(ind!=NULL)
        {
          id=ind->mem_id;
          ind=ind->next;
        }  
        fprintf(file, "%d",id);
        if (i < 5) fprintf(file, ",");
      }
     fprintf(file,"%d",temp->weekly_group_goal);
     fprintf(file, "\n");
     temp=temp->next;
  }
}       

void DeleteAllIndividual()
{
    individual *ptr;
    while(ind_lptr!=NULL)
    {
       ptr=ind_lptr;
       ind_lptr=ind_lptr->next;
       free(ptr);
    }
}

void DeleteAllGroup()
{
     group *ptr;
     member *temp;
     while(grp_lptr!=NULL)
     {
        ptr=grp_lptr;
        grp_lptr=grp_lptr->next;
        while(ptr->grp_mem_ptr!=NULL)
        {
            temp=ptr->grp_mem_ptr;
            ptr->grp_mem_ptr=ptr->grp_mem_ptr->next;
            free(temp);
        }
        free(ptr);
     }
}


int main() 
{

   status_code sc;
   sc=extract_ind_csv();
   sc=extract_grp_csv();
   printf("\nWELCOME TO THE STEP SYNC!!\n");
   char ans='y';
   int n,id1=0,id2=0;
   while(ans=='y')
   {
      printf("\nAvailable features: \n");
      printf("1)Add Person\n2)Create Group\n3)Top 3 in individuals\n4)Group Achievement\n5)Leader Board\n6)Individual Reward\n7)Delete Individual\n8)Delete Group\n9)Merge Groups\n10)Display Group Information\n11)Suggest goal update\n");
      printf("\nEnter the option number: ");
      scanf("%d",&n);
      switch(n) 
      {
        case 1:
            printf("Add Person\n");
            sc=add_person();
            break;
        case 2:
            printf("Create Group\n");
            sc=create_group();
            break;
        case 3:
            printf("Top 3 in individuals\n");
            print_top_3();
            break;
        case 4:
            printf("Group Achievement\n");
            printf("Enter the Group ID number to check group achievement: ");
            scanf("%d",&id1);
            sc=check_group_achievement(id1);
            break;
        case 5:
            printf("Leader Board\n");
            sc=generate_leader_board();
            break;
        case 6:
            printf("Individual Reward\n");
            printf("Enter the Individual ID to find the reward: ");
            scanf("%d",&id1);
            check_individual_rewards(id1);
            break;
        case 7:
            printf("Delete Individual\n");
            printf("Enter the Individual ID to delete the individual: ");
            scanf("%d",&id1);
            sc=delete_individual(id1);
            break;
        case 8:
            printf("Delete Group\n");
            printf("Enter the Group ID to delete the group: ");
            scanf("%d",&id1);
            sc=delete_group(id1);
            break;
        case 9:
            printf("Merge Groups\n");
            printf("Enter the Group ID 1 to merge the groups: ");
            scanf("%d",&id1);
            printf("Enter the Group ID 2 to merge the groups: ");
            scanf("%d",&id2);
            sc=merge_groups(id1,id2);
            break;
        case 10:
            printf("Display Group Information\n");
            printf("Enter the Group ID to display the information: ");
            scanf("%d",&id1);
            display_group_info(id1);
            break;
        case 11:
            printf("Suggest goal update\n");
            printf("Enter the individual ID for suggestion in goal updation: ");
            scanf("%d",&id1);
            sc=suggest_goal_update(id1);
            break;
        default:
            printf("Invalid option\n");
            // Handle invalid option
            break;
        
    }
    getchar();
    printf("\nWant to perform more operations?[y/n]\n");
    scanf("%c",&ans);    
  }  
  printf("\nThank You!!!....Keep walking:)\n");
  insert_ind_file();
  insert_grp_file();
  DeleteAllIndividual();
  DeleteAllGroup();
  return 0;
}
