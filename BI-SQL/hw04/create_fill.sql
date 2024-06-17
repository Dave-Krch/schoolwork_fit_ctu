drop table if exists trusted_people cascade;
drop table if exists crimes cascade;
drop table if exists places cascade;
drop table if exists suspect_activities cascade;
drop table if exists trusted_people_activities cascade;

create table trusted_people(id serial primary key, name varchar(64) not null , birth_date date not null);

create table places(name varchar(64) primary key);

create table crimes(date_commited date not null, description varchar(64) not null);

create table trusted_people_activities(id integer, from_date date not null, to_date date not null, place varchar(64));
alter table trusted_people_activities add constraint fk_trusted_person foreign key (id) references trusted_people(id) on delete cascade;
alter table trusted_people_activities add constraint fk_trusted_p_places foreign key (place) references places(name) on delete cascade;

create table suspect_activities(from_date date not null, to_date date not null, name_with varchar(64) not null, place varchar(64) not null);

INSERT INTO trusted_people VALUES (default, 'Adam', '1.1.1990');
INSERT INTO trusted_people VALUES (default, 'Bert', '3.5.1839');
INSERT INTO trusted_people VALUES (default, 'Cyril', '9.9.2020');

INSERT INTO places VALUES ('FIT');
INSERT INTO places VALUES ('lidl');
INSERT INTO places VALUES ('secret_hideout');
INSERT INTO places VALUES ('bank');
INSERT INTO places VALUES ('pub');

INSERT INTO trusted_people_activities VALUES (1, '1.1.2024', '1.10.2024', 'FIT');
INSERT INTO trusted_people_activities VALUES (1, '1.11.2024', '1.20.2024', 'lidl');

INSERT INTO trusted_people_activities VALUES (2, '1.1.2024', '1.10.2024', 'secret_hideout');
INSERT INTO trusted_people_activities VALUES (2, '1.11.2024', '1.20.2024', 'secret_hideout');

INSERT INTO trusted_people_activities VALUES (3, '1.1.2024', '1.10.2024', 'bank');
INSERT INTO trusted_people_activities VALUES (3, '1.11.2024', '1.20.2024', 'pub');

insert INTO crimes VALUES ('1.2.2024', 'unpaid tab'); -- ok alibi
insert INTO crimes VALUES ('1.15.2024', 'stabbed postman'); -- prekryvajici se alibi
insert INTO crimes VALUES ('1.18.2024', 'pirated movie'); -- vymyslene alibi
insert INTO crimes VALUES ('10.10.2024', 'stolen car'); -- neexistujici alibi

-- INSERT INTO suspect_activities VALUES ('1.1.2024', '1.10.2024', 'Adam', 'FIT');

-- INSERT INTO suspect_activities VALUES ('1.15.2024', '1.16.2024', 'Bert', 'secret_hideout');
-- INSERT INTO suspect_activities VALUES ('1.15.2024', '1.16.2024', 'Cyril', 'pub');

-- INSERT INTO suspect_activities VALUES ('1.17.2024', '1.19.2024', 'Cyril', 'FIT');

commit;