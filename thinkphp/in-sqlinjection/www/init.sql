USE `cat`;

CREATE TABLE IF NOT EXISTS `user` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `username` varchar(255) NOT NULL,
  `password` varchar(255) NOT NULL,
  PRIMARY KEY (`id`)
) AUTO_INCREMENT=1 ;

INSERT INTO `user` (`username`, `password`) VALUES
('admin', 'admin');
INSERT INTO `user` (`username`, `password`) VALUES
('test', 'test');