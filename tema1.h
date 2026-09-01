/*GREAVU ANDREEA - MARIA*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//structura pentru un echipaj
typedef struct unit {
	int id;
	char type;
	int availability;
} Tunit;
//structura pentru un incident
typedef struct incident {
	int id;
	char priority[7];
	char *description;
	char status[11];
} Tincident;
//structura pentru o interventie
typedef struct intervention {
	Tincident *incident;
	Tunit *unit;
} Tintervention;
//lista dublu inlantuita pentru incidente
typedef struct celula_incident {
	Tincident *info;
	struct celula_incident *prev, *urm;
} TCelula1, *TListaInc;
//lista dublu inlantuita pentru interventii
typedef struct celula_interventie {
	Tintervention *info;
	struct celula_interventie *prev, *urm;
} TCelula2, *TListaInt;
//structuri pentru o coada
typedef struct celQ {
	void *info;
	struct celQ *urm;
} TCelulaQ, *TListaQ;
typedef struct coada {
	TListaQ sf;
}TCoada;
//structura pentru o stiva
typedef struct celS {
	TCelula2 *info;
	struct celS *urm;
} TCelulaS, *TStiva;
//structura pentru gestionarea sistemului
typedef struct system {
	Tunit units[50];
	int nr_units;
	TListaInc incidents;
	TListaInt interventions;
	TCoada *queue_high, *queue_medium, *queue_low, *queue_available_units;
	TStiva undo;
}Tsystem;

TListaInc InitListaIncidente();
TCelula1 *AlocIncident(int id, char *priority, char *description);
void InsereazaSf(TListaInc s, TCelula1 *aux);
TCelulaQ *AlocCel(void *info);
TCoada *InitQ();
int IntrQ(TCoada *c, void *info);
void AddIncident(Tsystem *sys, int id, char *priority, char *description);
void CheckUnitsAvailability(Tsystem *sys, FILE *g);
void ShowUnit(Tsystem *sys, int id, FILE *g);
void ShowIncident(Tsystem *sys, int id, FILE *g);
void ShowInterventions(Tsystem *sys, FILE *g);
void *ExtrQ(TCoada *c);
TListaInt InitListaInterventii();
TCelula2 *AlocInterventie(Tincident *incident, Tunit *unit);
void InsInt(TListaInt s, TCelula2 *aux);
int Push(TStiva *vf, TCelula2 *x);
void Dispatch(Tsystem *sys, FILE *g);
int IntrincQ(TCoada *c, void *info);
int Pop(TStiva *vf, TCelula2 **x);
void UndoLastDispatch(Tsystem *sys, FILE *g);
void SolvedIncident(Tsystem *sys, int id, FILE *g);
void DistrQ(TCoada **c);
void DistrS(TStiva *s);
void Freemem(Tsystem *sys);