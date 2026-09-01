/*GREAVU ANDREEA-MARIA*/
#include "tema1.h"
//creem santinela pentru lista de incidente
TListaInc InitListaIncidente()
{
	TCelula1 *s = (TCelula1 *)malloc(sizeof(TCelula1));
	if(!s) {
		return NULL;
	}
	s->info = (Tincident*)malloc(sizeof(Tincident));
	if(!s->info) {
		free(s);
		return NULL;
	}
	s->info->id = 0;
	strcpy(s->info->priority, "low");
	s->info->description= (char*)malloc(strlen("test incident") + 1);
	strcpy(s->info->description, "test incident");
	strcpy(s->info->status, "solved");
	s->prev = s;
	s->urm = s;
	return s;
}
//creem incident nou
TCelula1 *AlocIncident(int id, char *priority, char *description)
{
	TCelula1 *aux = (TCelula1*)malloc(sizeof(TCelula1));
	if(!aux){
		return NULL;
	}
	aux->info = (Tincident*)malloc(sizeof(Tincident));
	if(!aux->info){
		free(aux);
		return NULL;
	}
	aux->info->id = id;
	strcpy(aux->info->priority, priority);
	aux->info->description = (char*)malloc(strlen(description) + 1);
	if(!aux->info->description) {
		free(aux->info);
		free(aux);
		return NULL;
	}
	strcpy(aux->info->description, description);
	strcpy(aux->info->status, "queued");
	return aux;
}
//adaugam la finalul listei de incidente
void InsereazaSf(TListaInc s, TCelula1 *aux)
{
	TCelula1 *ultim = s->prev;
	ultim->urm = aux;
	aux->urm = s;
	aux->prev = ultim;
	s->prev = aux;
}
//alocare celula coada
//folosesc aceeasi functie si pt cozile de prioritate si pt coada de echipaje motiv pt care am pus void *
TCelulaQ *AlocCel(void *info)
{
	TCelulaQ *aux = (TCelulaQ*)malloc(sizeof(TCelulaQ));
	if(!aux) {
		return NULL;
	}
	aux->info = info;
	aux->urm = NULL;
	return aux;
}
//initializare coada
TCoada *InitQ()
{
	TCoada *c = (TCoada *)malloc(sizeof(TCoada));
	if (!c){
		return NULL;
	}
	c->sf = NULL;
	return c;
}
//introducere in coada
int IntrQ(TCoada *c, void *info)
{
	TCelulaQ *aux = AlocCel(info);
	if(!aux) {
		return 0;
	}
	if(!c->sf) {
		aux->urm = aux;
		c->sf = aux;
	} else {
		aux->urm = c->sf->urm;
		c->sf->urm = aux;
		c->sf = aux;
	}
	return 1;
}
//adaugare incident
void AddIncident(Tsystem *sys, int id, char *priority, char *description)
{
	TCelula1 *aux = AlocIncident(id, priority, description);
	if(!aux) {
		return;
	}
	//punem incidentul la sfarsitul listei de incidente
	InsereazaSf(sys->incidents, aux);
	//dupa gradul de prioritate il punem in coada corecta
	if (strcmp(priority, "high") == 0) {
		IntrQ(sys->queue_high, aux);
	} else if (strcmp(priority, "medium") == 0) {
		IntrQ(sys->queue_medium, aux);
	} else IntrQ(sys->queue_low, aux);
}
//verificare echipaje disponibile
void CheckUnitsAvailability(Tsystem *sys, FILE *g)
{
	int count = 0;
	//parcurgem coada echiajelor si numaram cate sunt
	if (sys->queue_available_units->sf != NULL) {
		count++;
		TCelulaQ *aux = sys->queue_available_units->sf->urm;
		while (aux != sys->queue_available_units->sf) {
			count++;
			aux = aux->urm;
		}
	}
	fprintf(g, "Number of available units: %d\n", count);
}
//afisare detalii echipaj de interventie
void ShowUnit(Tsystem *sys, int id, FILE *g)
{
	for (int i = 0; i < sys->nr_units; i++) {
		if(sys->units[i].id == id) {
			if (sys->units[i].availability == 1) {
				fprintf(g, "Unit %d is type %c and is available\n", sys->units[i].id, sys->units[i].type);
			} else {
				fprintf(g, "Unit %d is type %c and is unavailable\n", sys->units[i].id, sys->units[i].type);
			}
			return;
		}
	}
	fprintf(g, "INVALID OPERATION! ERROR 404\n");
}
//afisare incident
void ShowIncident(Tsystem *sys, int id, FILE *g)
{
	//cautam dupa id si daca il gasim il afisam
	TCelula1 *aux = sys->incidents->urm;
	while (aux != sys->incidents) {
	if(aux->info->id == id) {
		fprintf(g, "Incident %d has %s priority, the following description: ",aux->info->id, aux->info->priority);
		fprintf(g, "%c", '"');
		fprintf(g, "%s", aux->info->description);
		fprintf(g, "%c", '"');
		fprintf(g, " and is %s\n", aux->info->status);
		return;
		}
		aux = aux->urm;
	}
	fprintf(g,"INVALID OPERATION! ERROR 404\n");
}
//afisare interventii
void ShowInterventions(Tsystem *sys, FILE *g)
{
	//daca lista de interventii e goala
	if (sys->interventions->urm == sys->interventions) {
		fprintf(g, "No intervention has been initiated\n");
		return;
	}
	TCelula2 *aux = sys->interventions->urm;
	while(aux != sys->interventions) {
		fprintf(g, "Incident %d was assigned to unit %d,"" and has the following status: ", aux->info->incident->id, aux->info->unit->id);
		fprintf(g, "%c", '"');
		fprintf(g, "%s", aux->info->incident->status);
		fprintf(g, "%c", '"');
		fprintf(g, "\n");
		aux = aux->urm;
	}
}
//extragere din coada
void *ExtrQ(TCoada *c)
{
	if(c->sf ==  NULL) {
		return NULL;
	}
	TCelulaQ *aux = c->sf->urm;
	void *info = aux->info;
	if (aux == c->sf) {
		c->sf = NULL;
	} else {
		c->sf->urm = aux->urm;
	}
	free(aux);
	return info;
}
//initializare lista interventii
TListaInt InitListaInterventii()
{
	TCelula2 *s = (TCelula2 *)malloc(sizeof(TCelula2));
	if(!s) {
	return NULL;
	}
	s->info = (Tintervention*)malloc(sizeof(Tintervention));
	if(!s->info) {
		free(s);
		return NULL;
	}
	s->info->incident = NULL;
	s->info->unit = NULL;
	s->prev = s;
	s->urm = s;
	return s;
}
//creem interventie noua
TCelula2 *AlocInterventie(Tincident *incident, Tunit *unit)
{
	TCelula2 *aux = (TCelula2*)malloc(sizeof(TCelula2));
	if(!aux){
		return NULL;
	}
	aux->info = (Tintervention*)malloc(sizeof(Tintervention));
	if(!aux->info){
		free(aux);
		return NULL;
	}
	aux->info->incident = incident;
	aux->info->unit = unit;
	return aux;
}
//adaugare la finalul listei de interventii
void InsInt(TListaInt s, TCelula2 *aux)
{
	TCelula2 *ultim = s->prev;
	ultim->urm = aux;
	aux->urm = s;
	aux->prev = ultim;
	s->prev = aux;
}
//inserare in stiva
int Push(TStiva *vf, TCelula2 *x)
{
	TCelulaS *aux = (TCelulaS*)malloc(sizeof(TCelulaS));
	if(!aux) {
		return 0;
	}
	aux->info = x;
	aux->urm = *vf;
	*vf = aux;
	return 1;
}
//functia pt dispatch
void Dispatch(Tsystem *sys, FILE *g)
{
	//daca nu avem incidente in cozi sau echipaje disponibile afisam eroare
	if((sys->queue_high->sf == NULL && sys->queue_low->sf == NULL && sys->queue_medium->sf == NULL) || sys->queue_available_units->sf == NULL) {
		fprintf(g, "INVALID OPERATION! ERROR 404\n");
		return;
	}
	TCelula1 *incident = NULL;
	//cautam primul incident din prima coada care nu e goala si il extragem
	if(sys->queue_high->sf != NULL) {
		incident = (TCelula1*)ExtrQ(sys->queue_high);
	} else if(sys->queue_medium->sf != NULL) {
		incident = (TCelula1*)ExtrQ(sys->queue_medium);
	} else if(sys->queue_low->sf != NULL) {
		incident = (TCelula1*)ExtrQ(sys->queue_low);
	}
	//scoatem primul echipaj din coada de echipaje disponibile
	Tunit *available_unit = (Tunit*)ExtrQ(sys->queue_available_units);
	//incidentul il marcam ca fiind intervented si echipajul ca fiind nedisponibil
	strcpy(incident->info->status, "intervened");
	available_unit->availability = 0;
	//creem interventia dintre incident si echipaj
	TCelula2 *aux = AlocInterventie(incident->info, available_unit);
	//adaugam ce am creat la sfarsitul listei de interventii
	InsInt(sys->interventions, aux);
	//salvam interventia in stiva
	Push(&sys->undo, aux);
}
//adaugare la inceputul unei cozi:
int IntrincQ(TCoada *c, void *info)
{
	TCelulaQ *aux = AlocCel(info);
	if (!aux) {
		return 0;
	}
	if(!c->sf) {
		aux->urm = aux;
		c->sf = aux;
	} else {
		aux->urm = c->sf->urm;
		c->sf->urm = aux;
	}
	return 1;
}

//extragere din stiva
int Pop(TStiva *vf, TCelula2 **x)
{
	TStiva aux;
	if(*vf == NULL) {
		return 0;
	}
	*x = (*vf)->info;
	aux = *vf;
	*vf = aux->urm;
	free(aux);
	return 1;
}
//functia pt undo dispatch
void UndoLastDispatch(Tsystem *sys, FILE *g)
{
	TCelula2 *find = NULL;
	TStiva aux = NULL; //stiva pentru a pune interventiile finalizate
	//cautam in stiva ultima interventie care nu a fost finalizata 
	while(sys->undo != NULL) {
		TCelula2 *tmp = NULL;
		Pop(&sys->undo, &tmp);//extragem din stiva 
		if(strcmp(tmp->info->incident->status, "intervened") == 0) {
			//am gasit interventia
			find = tmp;
			break;
		}
		//o punem in stiva auxiliara pt ca inseamna ca e rezolvata
		Push(&aux, tmp);
	}
	//punem inapoi in stiva pt undo interventiile rezolvate
	while(aux) {
		TCelula2 *t = NULL;
		Pop(&aux, &t);
		Push(&sys->undo, t);
	}
	//daca nu am gasit nimic afisam o eroare
	if (!find) {
		fprintf(g, "INVALID OPERATION! ERROR 404\n");
		return;
	}
	//incidentul devine iar queued
	strcpy(find->info->incident->status, "queued");
	//cautam incidentul dupa id
	TCelula1 *auxx = sys->incidents->urm;
	while(auxx != sys->incidents) {
		if(auxx->info->id == find->info->incident->id) {
			break;
		}
		auxx = auxx->urm;
	}
	//introducem inapoi incidentul la inceputul cozii corespunzatoare lui
	if(strcmp(find->info->incident->priority, "high") == 0) {
		IntrincQ(sys->queue_high, auxx);
	} else if(strcmp(find->info->incident->priority, "medium") == 0) {
		IntrincQ(sys->queue_medium, auxx);
	} else if(strcmp(find->info->incident->priority, "low") == 0) {
		IntrincQ(sys->queue_low, auxx);
	}
	//echipajul corespunzator devine iar disponibil si il punem la finalul cozii de echipaje
	find->info->unit->availability = 1;
	IntrQ(sys->queue_available_units, find->info->unit);
	//scoatem interventia din lista de interventii
	find->prev->urm = find->urm;
	find->urm->prev = find->prev;
	free(find->info);
	free(find);
}
//rezolvare incident
void SolvedIncident(Tsystem *sys, int id, FILE *g)
{
	//cautam incidentul in functie de id
	TCelula1 *aux = sys->incidents->urm;
	while (aux != sys->incidents) {
		if (aux->info->id == id) {
			break;
		}
		aux = aux->urm;
	}
	//daca avem un id invalid sau incidentul dat are alt status fata de intervented afisam eroare
	if (!aux || strcmp(aux->info->status, "intervened") != 0) {
		fprintf(g, "INVALID OPERATION! ERROR 404\n");
		return;
	}
	TCelula2 *auxx = sys->interventions->urm;
	while(auxx != sys->interventions) {
		if(auxx->info->incident->id == id) {
			break;
		}
		auxx = auxx->urm;
	}
	//interventia devine rezolvata
	strcpy(aux->info->status, "solved");
	//echipajul devine iarasi disponibil si il introducem la sf cozii de echipaje
	auxx->info->unit->availability = 1;
	IntrQ(sys->queue_available_units, auxx->info->unit);
}
//fc care elibereaza o coada
void DistrQ(TCoada **c)
{
	if ((*c)->sf != NULL) {
		TListaQ aux = (*c)->sf->urm;
		(*c)->sf->urm = NULL;
		while (aux) {
			TListaQ t = aux;
			aux = aux->urm;
			free(t);
		}
	}
	free(*c);
	*c = NULL;
}
//fc care elibereaza o stiva
void DistrS(TStiva *s)
{
	while(*s != NULL) {
		TCelula2 *t = NULL;
		Pop(s, &t);
	}
}
//fc pt eliberarea memoriei
void Freemem(Tsystem *sys)
{
	//eliberam lista de interventii
	TCelula2 *aux = sys->interventions->urm;
	while(aux != sys->interventions) {
		TCelula2 *t = aux;
		aux = aux->urm;
		free(t->info);
		free(t);
	}
	free(sys->interventions->info);
	free(sys->interventions);
	//elberam lista de incidente
	TCelula1 *aux2 = sys->incidents->urm;
	while(aux2 != sys->incidents) {
		TCelula1 *t = aux2;
		aux2 = aux2->urm;
		free(t->info->description);
		free(t->info);
		free(t);
	}
	free(sys->incidents->info->description);
	free(sys->incidents->info);
	free(sys->incidents);
	//eliberam cozile de asteptare si stiva
	DistrQ(&sys->queue_high);
	DistrQ(&sys->queue_medium);
	DistrQ(&sys->queue_low);
	DistrQ(&sys->queue_available_units);
	DistrS(&sys->undo);
}