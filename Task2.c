/*
 * Task 2 - Memory Management Simulation
 * Operating Systems Coursework
 * Author: Uvica Shrestha
 */

#include <stdio.h>

#define MAX_PAGES 50
#define MAX_FRAMES 10

// Reference string
int pages[] = {7,0,1,2,0,3,0,4,2,3,0,3,2};

// Number of page references
int n = 13;

// Number of frames
int frames = 3;

// Configurable page size (KB)
int pageSize = 4;

int fifoHits, fifoFaults;
int lruHits, lruFaults;

void fifo()
{
    int memory[MAX_FRAMES];

    for(int i = 0; i < frames; i++)
    {
        memory[i] = -1;
    }

    int pointer = 0;
    int hits = 0;
    int faults = 0;

    printf("\nFIFO PAGE REPLACEMENT\n");
    printf("---------------------\n");

        for(int i = 0; i < n; i++)
    {
        int page = pages[i];
        int found = 0;

        for(int j = 0; j < frames; j++)
        {
            if(memory[j] == page)
            {
                found = 1;
                hits++;
                break;
            }
        }
                if(!found)
        {
            memory[pointer] = page;
            pointer = (pointer + 1) % frames;
            faults++;

            printf("Page %d -> Fault\n", page);
        }
        else
        {
            printf("Page %d -> Hit\n", page);
        }
                printf("Frames : ");

        for(int j = 0; j < frames; j++)
        {
            if(memory[j] == -1)
                printf("- ");
            else
                printf("%d ", memory[j]);
        }

        printf("\n\n");
    }

        printf("Total Hits   : %d\n", hits);
    printf("Total Faults : %d\n", faults);

    float hitRatio = (float)hits / n;
    float missRatio = (float)faults / n;

    printf("Hit Ratio    : %.2f%%\n", hitRatio * 100);
    printf("Miss Ratio   : %.2f%%\n", missRatio * 100);
    fifoHits = hits;
fifoFaults = faults;
}
// lru
void lru()
{
    int memory[MAX_FRAMES];
    int recent[MAX_FRAMES];

    for(int i = 0; i < frames; i++)
    {
        memory[i] = -1;
        recent[i] = -1;
    }

    int hits = 0;
    int faults = 0;

    printf("\nLRU PAGE REPLACEMENT\n");
    printf("---------------------\n");

        for(int i = 0; i < n; i++)
    {
        int page = pages[i];
        int found = 0;

        for(int j = 0; j < frames; j++)
        {
            if(memory[j] == page)
            {
                found = 1;
                hits++;
                recent[j] = i;
                break;
            }
        }

                if(!found)
        {
            faults++;

            int empty = -1;

            for(int j = 0; j < frames; j++)
            {
                if(memory[j] == -1)
                {
                    empty = j;
                    break;
                }
            }
                        if(empty != -1)
            {
                memory[empty] = page;
                recent[empty] = i;
            }

                        else
            {
                int lruIndex = 0;

                for(int j = 1; j < frames; j++)
                {
                    if(recent[j] < recent[lruIndex])
                    {
                        lruIndex = j;
                    }
                }

                memory[lruIndex] = page;
                recent[lruIndex] = i;
            }

            printf("Page %d -> Fault\n", page);
        }
        else
        {
            printf("Page %d -> Hit\n", page);
        }

                printf("Frames : ");

        for(int j = 0; j < frames; j++)
        {
            if(memory[j] == -1)
                printf("- ");
            else
                printf("%d ", memory[j]);
        }

        printf("\n\n");
    }
        printf("Total Hits   : %d\n", hits);
    printf("Total Faults : %d\n", faults);

    float hitRatio = (float)hits / n;
    float missRatio = (float)faults / n;

    printf("Hit Ratio    : %.2f%%\n", hitRatio * 100);
    printf("Miss Ratio   : %.2f%%\n", missRatio * 100);

    lruHits = hits;
lruFaults = faults;
}

void compareAlgorithms()
{
    printf("\nPERFORMANCE COMPARISON\n");
    printf("----------------------\n");

    printf("FIFO\n");
    printf("Hits   : %d\n", fifoHits);
    printf("Faults : %d\n\n", fifoFaults);

    printf("LRU\n");
    printf("Hits   : %d\n", lruHits);
    printf("Faults : %d\n\n", lruFaults);

    if(lruFaults < fifoFaults)
    {
        printf("LRU performed better because it generated fewer page faults.\n");
    }
    else if(fifoFaults < lruFaults)
    {
        printf("FIFO performed better because it generated fewer page faults.\n");
    }
    else
    {
        printf("Both algorithms produced the same number of page faults.\n");
    }
}
int main()
{
    printf("MEMORY MANAGEMENT SIMULATOR\n");
    printf("----------------------------\n");

    printf("Page Size : %d KB\n", pageSize);
    printf("Frames    : %d\n", frames);

    printf("Reference String : ");

    for(int i = 0; i < n; i++)
    {
        printf("%d ", pages[i]);
    }

    printf("\n");
    
    fifo();

    lru();

compareAlgorithms();



    return 0;
}
