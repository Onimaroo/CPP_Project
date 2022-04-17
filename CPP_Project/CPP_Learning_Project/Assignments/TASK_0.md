# Se familiariser avec l'existant

## A- Exécution

Compilez et lancez le programme.

Allez dans le fichier `tower_sim.cpp` et recherchez la fonction responsable de gérer les inputs du programme.
> create_keystrokes().

Sur quelle touche faut-il appuyer pour ajouter un avion ?
> La touche "c".

Comment faire pour quitter le programme ?
> Avec le bouton "x" ou "q".

A quoi sert la touche 'F' ?
> Elle met le programme en plein écran.

Ajoutez un avion à la simulation et attendez.
Que est le comportement de l'avion ?
> L'avion vole autour de l'aéroport avant de stationner.
Quelles informations s'affichent dans la console ?
> Des messages de prévention pour dire que l'avion ajoutée va atterir.

Ajoutez maintenant quatre avions d'un coup dans la simulation.
Que fait chacun des avions ?
> Un avion atterit et les autres continuent de voler tant que l'avion n'ait pas repris son vol.

## B- Analyse du code

Listez les classes du programme à la racine du dossier src/.
Pour chacune d'entre elle, expliquez ce qu'elle représente et son rôle dans le programme.

> Aircraft -> Designe un avion.

> Airport -> Designe l'aéroport.

> AirportType -> Designe le type d'aéroport. Elle a été crée pour les différencier entre eux.

> Terminal -> Désigne un des terminaux de l'aéroport.

> Tower -> Désigne une tour qui se charge de l'entretien des avions.

> TowerSimulation -> Fait la simulation, tel que créer des avions, gérer les évènements, etc.

Pour les classes `Tower`, `Aircaft`, `Airport` et `Terminal`, listez leurs fonctions-membre publiques et expliquez précisément à quoi elles servent.
Réalisez ensuite un schéma présentant comment ces différentes classes intéragissent ensemble.

> Classe Tower:
(Tower::get_circle() -> Retourne les coordonnées d'un cercle d'atterrissage.)
Tower::get_instructions(Aircraft& aircraft) -> Produit des instructions pour l'aéroport.
Tower::arrived_at_terminal(const Aircraft& aircraft) -> Est appelé quand un avion atterit dans un terminal. Fait les actions correspondantes comme démarrer le service du terminal.

> Classe Aircraft:
Aircraft -> Son constructeur.
Aircraft::get_flight_num() -> Retourne le numéro de vol.
Aircraft::distance_to(const Point3D& p) -> Retourne la distance qu'il reste à parcourir à l'avion pour se rendre au pont donné en paramètre.
Aircraft::display() -> Dessine l'avion dans l'interface graphique.
Aircraft::move() -> Déplace l'avion.

> Classe Airport:
(private: Airport::reserve_terminal(Aircraft& aircraft) -> Réserve un terminal pour l'avion donné en paramètre.)
Airport -> Son constructeur.
Airport::get_tower() -> Retourne la tour.
Airport::display() -> Affiche l'aéroport.
Airport::move() -> Met à jour les services des terminaux.

> Classe Terminal:
Terminal -> Son constructeur.
Terminal::in_use() -> Vérifie si le terminal n'est pas déjà utilisé par un avion.
Terminal::is_servicing() -> Vérifie si le terminal n'est pas déjà en plein service
Terminal::assign_craft(const Aircraft& aircraft) -> Assigne un avion au terminal.
Terminal::start_service(const Aircraft& aircraft) -> Démarre le service.
Terminal::finish_service() -> Termine le service.
Terminal::move() -> Avance dans le progrès du service.



Quelles classes et fonctions sont impliquées dans la génération du chemin d'un avion ?
> La classe Airport avec start_path(), la classe WayPoint et la classe Aircraft avec la fonction distance_to()

Quel conteneur de la librairie standard a été choisi pour représenter le chemin ?
Expliquez les intérêts de ce choix.
> Le conteneur choisi est std::deque, qui est une queue avec une double fin.


## C- Bidouillons !

1) Déterminez à quel endroit du code sont définies les vitesses maximales et accélération de chaque avion.
> aircraft_types.

Le Concorde est censé pouvoir voler plus vite que les autres avions.
Modifiez le programme pour tenir compte de cela.
> Si le Concorde va trop vite, l'avion crashe et le programme aussi.

2) Identifiez quelle variable contrôle le framerate de la simulation.
> La variable DEFAULT_TICKS_PER_SEC.

Ajoutez deux nouveaux inputs au programme permettant d'augmenter ou de diminuer cette valeur.
```cpp 
void TowerSimulation::create_keystrokes() const
{
    GL::keystrokes.emplace('a', []() { GL::ticks_per_sec += 10; });
    GL::keystrokes.emplace('d', []() { GL::ticks_per_sec -= 10; });
}
```
Essayez maintenant de mettre en pause le programme en manipulant ce framerate. Que se passe-t-il ?\
> On modifie d'abord l'interface OpenGL:

```cpp
void timer(const int step)
{
    if(ticks_per_sec != 0) {
        for (auto& item : move_queue)
        {
            item->move();
        }
        glutPostRedisplay();
        glutTimerFunc(1000u / ticks_per_sec, timer, step + 1);
    }
    else {
        glutPostRedisplay();
        glutTimerFunc(1000u / 100, timer, step + 1);
    }
}
```
> Puis on ajoute la fonction suivante:
```cpp 
void TowerSimulation::create_keystrokes() const
{
    GL::keystrokes.emplace('p', []() { GL::ticks_per_sec = 0; });
}
```
> Quand on met en pause la framerate, la framerate est remis à 0, donc les augmentations et diminutions de vitesse d'avant ne sont plus pris en compte.

Ajoutez une nouvelle fonctionnalité au programme pour mettre le programme en pause, et qui ne passe pas par le framerate.
> On rajoute un booléen is_paused pour ne pas changer la framerate au moment de la pause.

3) Identifiez quelle variable contrôle le temps de débarquement des avions et doublez-le.
> La variable SERVICE_CYCLES, qui détermine le temps de service des terminaux après qu'un avion ait atterit dessus.

4) Lorsqu'un avion a décollé, il réattérit peu de temps après.
Faites en sorte qu'à la place, il soit retiré du programme.\
Indices :\
A quel endroit pouvez-vous savoir que l'avion doit être supprimé ?\
> Le moment où l'avion doit être supprimé est au moment où le terminal finit son service. Il faut alors changer Aircraft::move()

Pourquoi n'est-il pas sûr de procéder au retrait de l'avion dans cette fonction ?
> Cependant, il n'est pas safe de procéder au retrait de l'avion dans cette fonction car on risque de casser le foreach.

A quel endroit de la callstack pourriez-vous le faire à la place ?\
> Dans la fonction timer().

Que devez-vous modifier pour transmettre l'information de la première à la seconde fonction ?

```cpp 
timer();
for(item:move_queue) {
    item->move();
}
```

5) Lorsqu'un objet de type `Displayable` est créé, il faut ajouter celui-ci manuellement dans la liste des objets à afficher.
Il faut également penser à le supprimer de cette liste avant de le détruire.
Faites en sorte que l'ajout et la suppression de `display_queue` soit "automatiquement gérée" lorsqu'un `Displayable` est créé ou détruit.
Pourquoi n'est-il pas spécialement pertinent d'en faire de même pour `DynamicObject` ?

6) La tour de contrôle a besoin de stocker pour tout `Aircraft` le `Terminal` qui lui est actuellement attribué, afin de pouvoir le libérer une fois que l'avion décolle.
Cette information est actuellement enregistrée dans un `std::vector<std::pair<const Aircraft*, size_t>>` (size_t représentant l'indice du terminal).
Cela fait que la recherche du terminal associé à un avion est réalisée en temps linéaire, par rapport au nombre total de terminaux.
Cela n'est pas grave tant que ce nombre est petit, mais pour préparer l'avenir, on aimerait bien remplacer le vector par un conteneur qui garantira des opérations efficaces, même s'il y a beaucoup de terminaux.\
Modifiez le code afin d'utiliser un conteneur STL plus adapté. Normalement, à la fin, la fonction `find_craft_and_terminal(const Aicraft&)` ne devrait plus être nécessaire.

## D- Théorie

1) Comment a-t-on fait pour que seule la classe `Tower` puisse réserver un terminal de l'aéroport ?

2) En regardant le contenu de la fonction `void Aircraft::turn(Point3D direction)`, pourquoi selon-vous ne sommes-nous pas passer par une réference ?
Pensez-vous qu'il soit possible d'éviter la copie du `Point3D` passé en paramètre ?

## E- Bonus

Le temps qui s'écoule dans la simulation dépend du framerate du programme.
La fonction move() n'utilise pas le vrai temps. Faites en sorte que si.
Par conséquent, lorsque vous augmentez le framerate, la simulation s'exécute plus rapidement, et si vous le diminuez, celle-ci s'exécute plus lentement.

Dans la plupart des jeux ou logiciels que vous utilisez, lorsque le framerate diminue, vous ne le ressentez quasiment pas (en tout cas, tant que celui-ci ne diminue pas trop).
Pour avoir ce type de résultat, les fonctions d'update prennent généralement en paramètre le temps qui s'est écoulé depuis la dernière frame, et l'utilise pour calculer le mouvement des entités.

Recherchez sur Internet comment obtenir le temps courant en C++ et arrangez-vous pour calculer le dt (delta time) qui s'écoule entre deux frames.
Lorsque le programme tourne bien, celui-ci devrait être quasiment égale à 1/framerate.
Cependant, si le programme se met à ramer et que la callback de glutTimerFunc est appelée en retard (oui oui, c'est possible), alors votre dt devrait être supérieur à 1/framerate.

Passez ensuite cette valeur à la fonction `move` des `DynamicObject`, et utilisez-la pour calculer les nouvelles positions de chaque avion.
Vérifiez maintenant en exécutant le programme que, lorsque augmentez le framerate du programme, vous n'augmentez pas la vitesse de la simulation.

Ajoutez ensuite deux nouveaux inputs permettant d'accélérer ou de ralentir la simulation.
