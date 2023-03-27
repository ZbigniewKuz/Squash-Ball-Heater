CREATE TABLE `Machine` (
  `id` int PRIMARY KEY AUTO_INCREMENT,
  `owner_id` int,
  `addres` varchar(255),
  `temperature` double
);

CREATE TABLE `Action` (
  `action_id` int PRIMARY KEY AUTO_INCREMENT,
  `machine_id` int,
  `status` bool,
  `start` bool,
  `stop` bool,
  `hard_stop` bool,
  `stop_after_end` bool,
  `action_temperature` double,
  `is_ball_in` bool
);

CREATE TABLE `User` (
  `id` int PRIMARY KEY AUTO_INCREMENT,
  `name` varchar(255)
);

ALTER TABLE `Action` ADD FOREIGN KEY (`machine_id`) REFERENCES `Machine` (`id`);

ALTER TABLE `Machine` ADD FOREIGN KEY (`owner_id`) REFERENCES `User` (`id`);
