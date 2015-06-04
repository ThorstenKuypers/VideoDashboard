CREATE TABLE [dbo].[BetaTesters] (
    [Id]           INT         IDENTITY (1, 1) NOT NULL,
    [name]         NCHAR (128) NULL,
    [email]        NCHAR (260) NULL,
    [code]         NCHAR (20)  NULL,
    [ip]           NCHAR (25)  NULL,
    [registerDate] BIGINT      NULL,
    PRIMARY KEY CLUSTERED ([Id] ASC)
);

