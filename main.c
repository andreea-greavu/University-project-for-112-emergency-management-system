/*GREAVU ANDREEA - MARIA*/
#include "tema1.h"
//initializam listele cozile si stiva
void Initializari(Tsystem *sys)
{
	sys->incidents = InitListaIncidente();
	sys->interventions = InitListaInterventii();
	sys->queue_high = InitQ();
	sys->queue_medium = InitQ();
	sys->queue_low = InitQ();
	sys->queue_available_units = InitQ();
	sys->undo = NULL;
}
//rezolvam fiecare task in parte
void Resolve_task(FILE *f, FILE *g, Tsystem *sys, int n)
{
	char comm[100];
	for (int i = 0 ; i < n; i++){
		fscanf(f, "%s", comm);
		if (strcmp(comm,"ADD_INCIDENT") == 0) {
			int id;
			char priority[7], description[256];
			char command[10000];
			//citim id-ul si gradul de prioritizare
			fscanf(f,"%d %s", &id, priority);
			//cautam ce se afla intre ghilimele ca sa putem afla descrierea incidentului
			fgets(command,sizeof(command), f);
			char *gs = strchr(command, '"');
			gs++;
			char *gf = strchr(gs,'"');
			*gf = '\0';
			strcpy(description, gs);
			AddIncident(sys, id, priority, description);
		} else if (strcmp(comm,"CHECK_UNITS_AVAILABILITY") == 0) {
			CheckUnitsAvailability(sys, g);
		} else if (strcmp(comm,"DISPATCH") == 0) {
			Dispatch(sys,g);
		} else if (strcmp(comm,"UNDO_LAST_DISPATCH") == 0) {
			UndoLastDispatch(sys, g);
		} else if (strcmp(comm,"SOLVED_INCIDENT") == 0) {
			int id;
			fscanf(f, "%d", &id);
			SolvedIncident(sys, id,g);
		} else if (strcmp(comm,"SHOW_UNIT") == 0) {
			int id;
			fscanf(f, "%d", &id);
			ShowUnit(sys, id,g);			
		} else if (strcmp(comm,"SHOW_INCIDENT") == 0) {
			int id;
			fscanf(f, "%d", &id);
			ShowIncident(sys, id,g);
		} else if (strcmp(comm,"SHOW_INTERVENTIONS") == 0) {
			ShowInterventions(sys, g);
		}
	}
}
int main(void)
{
	FILE *f = fopen("tema1.in", "r");
	FILE *g = fopen("tema1.out", "w");

	Tsystem *sys = (Tsystem*)malloc(sizeof(Tsystem));
	if(!sys) {
		return -1;
	}

	fscanf(f, "%d", &sys->nr_units);
	Initializari(sys);
	for (int i = 0; i < sys->nr_units; i++) {
		fscanf(f, "%d %c", &sys->units[i].id, &sys->units[i].type);
		//marcam fiecare echipaj ca fiind disponibil
		sys->units[i].availability = 1;
		//adaugam in coada de echipaje fiecare echipaj citit
		IntrQ(sys->queue_available_units, &sys->units[i]);
	}
	//citim numarul de operatii si le executam
	int n;
	fscanf(f, "%d", &n);
	Resolve_task(f, g, sys, n);
	//eliberam memoria
	Freemem(sys);
	free(sys);
	fclose(f);
	fclose(g);
	return 0;
}