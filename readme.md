# WEBSERV

## Comprehension des commandes

- epoll : permet de gerer chaque fd concernant chaque requete
    
    struct epoll_event {
        uint32_t      events;  /* Epoll events */
        epoll_data_t  data;    /* User data variable */
    };

    union epoll_data {
        void     *ptr;
        int       fd;
        uint32_t  u32;
        uint64_t  u64;
    };


## NOTIONS HTTP

- Comprendre la RFC et implementer les differentes fonctionnalites de cette norme

### FORMAT DE BASE

Ligne de début (start-line)
En-têtes (headers)
Ligne vide (CRLF)
Corps du message (body) optionnel

### REQUETE

GET / HTTTP/1.1

premier argument: methode (post ou get)
deuxieme argument: chemin apres mon nom de domaine
troisieme argument: version HTTP

**En-Tetes requete**

Host (obligatoire)
User-Agent
Accept
Accept-Language
Accept-Encoding
Authorization

### REPONSE


**En-Tetes reponses**

Server
Content-Type
Content-Length
Content-Encoding
Location

### CODE ERREUR ET SUCCES

1xx : Information
2xx : Succès
3xx : Redirection
4xx : Erreur Client
5xx : Erreur Serveur

## FONCTIONNALITES A IMPLEMENTER

- Repertoire de fichiers statiques
- Redirection HTTP
- Accepter des fichiers

## CGI

### FONCTIONNALITES

- Implementer un CGI suivant l'extension du fichier demande.
- Le faire fonctionner avec GET et POST.

### EXPLICATIONS

- C'est une option sur un serveur qui va permettre de gerer des requetes specifiques comme des passages de script en parametre

## CONFIGURATION FILE

### CARACTERISTIQUES

- Si pas de server_name =>
        - repond aux requetes provenant de toute type de domaine.
        - Sinon je specifie pour savoir sur quel domaine j'ecoute
- Premier serveur par default si la requete ne correspond a aucun domaine
- default page error 
        => dans server et location
- limiter la taille du body des clients
        => dans server et location
- Activer ou desactiver le listing des repertoires:
        => dans location
- set un fichier par defaut si la requete est un repertoire.
        => 

### PARSING CONF

- parsing a partir du fichier de configuration.
- un bloc http 
    => un nombre de server dans ce http
        => redirection
        => location
        => root
- class: 
        - http
        - server
        - location

- dans http: 
        => server(plusieurs)
- dans server:
        => listen (port)
        => server_name 
        => root
        => index
        => return (redirection)
        => location
        => error_page
        => client_max_body_size 
- dans location:
        => index
        => return (redirection)
        => error_page
        => client_max_body_size
        => autoindex on

- obligation d'avoir un server et une location