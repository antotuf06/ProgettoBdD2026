--1.ARENA
CREATE TABLE ARENA (
    NomeArena VARCHAR(100) PRIMARY KEY,
    Citta VARCHAR(50) NOT NULL
);

--2.ALLENATORE
CREATE TABLE ALLENATORE (
    IDAllenatore INT PRIMARY KEY,
    Nome VARCHAR(50) NOT NULL,
    Cognome VARCHAR(50) NOT NULL,
    CONSTRAINT CHK_IDAllenatore CHECK (IDAllenatore BETWEEN 100 AND 999)
);

--3.ARBITRO
CREATE TABLE ARBITRO (
    IDArbitro SERIAL PRIMARY KEY,
    Nome VARCHAR(50) NOT NULL,
    Cognome VARCHAR(50) NOT NULL,
    Anni_Esperienza INT DEFAULT 0 CHECK (Anni_Esperienza >= 0),
    Specializzazione VARCHAR(20) CHECK (Specializzazione IN ('TAVOLO', 'REPLAY', 'CAMPO'))
);

--4.SQUADRA
CREATE TABLE SQUADRA (
    Nome_Squadra VARCHAR(50) PRIMARY KEY,
    Citta VARCHAR(50) NOT NULL,
    Nome_Arena VARCHAR(100) NOT NULL,
    ID_Allenatore INT UNIQUE,
    CONSTRAINT FK_ArenaSquadra FOREIGN KEY (Nome_Arena) REFERENCES ARENA(NomeArena),
    CONSTRAINT FK_AllenatoreSquadra FOREIGN KEY (ID_Allenatore) REFERENCES ALLENATORE(IDAllenatore)
);

--5.GIOCATORI
CREATE TABLE GIOCATORI (
    IDGiocatore INT PRIMARY KEY,
    Nome VARCHAR(50) NOT NULL,
    Cognome VARCHAR(50) NOT NULL,
    Ruolo VARCHAR(10) NOT NULL CHECK (Ruolo IN ('Ala', 'Guardia', 'Pivot')),
    Altezza INT NOT NULL CHECK (Altezza BETWEEN 150 AND 250),
    Peso INT NOT NULL CHECK (Peso BETWEEN 50 AND 200),
    Nome_Squadra VARCHAR(50) NOT NULL,
    CONSTRAINT FK_SquadraGiocatore FOREIGN KEY (Nome_Squadra) REFERENCES SQUADRA(Nome_Squadra),
    CONSTRAINT CHK_IDGiocatore CHECK (IDGiocatore BETWEEN 1000 AND 9999)
);

--6.PARTITA
CREATE TABLE PARTITA (
    IDPartita SERIAL PRIMARY KEY,
    DataOra TIMESTAMP NOT NULL,
    Squadra_Casa VARCHAR(50) NOT NULL,
    Squadra_Ospite VARCHAR(50) NOT NULL,
    Nome_Arena VARCHAR(100) NOT NULL,
    PtCasa INT DEFAULT 0,
    PtOspite INT DEFAULT 0,
    CONSTRAINT FK_SquadraCasa FOREIGN KEY (Squadra_Casa) REFERENCES SQUADRA(Nome_Squadra),
    CONSTRAINT FK_SquadraOspite FOREIGN KEY (Squadra_Ospite) REFERENCES SQUADRA(Nome_Squadra),
    CONSTRAINT FK_ArenaPartita FOREIGN KEY (Nome_Arena) REFERENCES ARENA(NomeArena),
    CONSTRAINT CHK_SquadreDiverse CHECK (Squadra_Casa <> Squadra_Ospite)
);

--7.DIREZIONE 
CREATE TABLE DIREZIONE (
    ID_Partita INT,
    ID_Arbitro INT,
    Ruolo_Gara VARCHAR(20) CHECK (Ruolo_Gara IN ('TAVOLO', 'REPLAY', 'CAMPO')),
    PRIMARY KEY (ID_Partita, ID_Arbitro),
    FOREIGN KEY (ID_Partita) REFERENCES PARTITA(IDPartita) ON DELETE CASCADE,
    FOREIGN KEY (ID_Arbitro) REFERENCES ARBITRO(IDArbitro) ON DELETE CASCADE
);