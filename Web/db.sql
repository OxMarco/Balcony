CREATE TABLE logs (
  n bigint NOT NULL AUTO_INCREMENT,
  topic varchar(100) NOT NULL,
  msg varchar(100) NOT NULL,
  PRIMARY KEY (n)
);
