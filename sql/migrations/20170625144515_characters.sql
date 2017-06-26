INSERT INTO `migrations` VALUES ('20170625144515');
ALTER TABLE character_aura
ADD COLUMN bonus0 INT AFTER basepoints2,
ADD COLUMN bonus1 INT AFTER bonus0,
ADD COLUMN bonus2 INT AFTER bonus1,
ADD COLUMN bonus_pct0 FLOAT AFTER bonus2,
ADD COLUMN bonus_pct1 FLOAT AFTER bonus_pct0,
ADD COLUMN bonus_pct2 FLOAT AFTER bonus_pct1,
ADD COLUMN flat0 INT AFTER bonus_pct2,
ADD COLUMN flat1 INT AFTER flat0,
ADD COLUMN flat2 INT AFTER flat1;
UPDATE character_aura SET bonus_pct0 = 1, bonus_pct1 = 1, bonus_pct2 = 1;
ALTER TABLE pet_aura
ADD COLUMN bonus0 INT AFTER basepoints2,
ADD COLUMN bonus1 INT AFTER bonus0,
ADD COLUMN bonus2 INT AFTER bonus1,
ADD COLUMN bonus_pct0 FLOAT AFTER bonus2,
ADD COLUMN bonus_pct1 FLOAT AFTER bonus_pct0,
ADD COLUMN bonus_pct2 FLOAT AFTER bonus_pct1,
ADD COLUMN flat0 INT AFTER bonus_pct2,
ADD COLUMN flat1 INT AFTER flat0,
ADD COLUMN flat2 INT AFTER flat1;
UPDATE pet_aura SET bonus_pct0 = 1, bonus_pct1 = 1, bonus_pct2 = 1;
