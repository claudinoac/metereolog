CREATE TABLE IAM_USER (
    id int PRIMARY KEY,
    first_name varchar,
    last_name varchar,
    email varchar,
    password varchar,
    is_staff boolean,
    is_active boolean,
    organization_id int
);

CREATE TABLE IAM_ORGANIZATION (
    id int PRIMARY KEY,
    identifier uuid,
    is_active boolean,
    name varchar
);

CREATE TABLE IAM_ROLE (
    id int PRIMARY KEY,
    name varchar,
    is_active boolean
);

CREATE TABLE DASHBOARD_DASHBOARD (
    name varchar,
    id int PRIMARY KEY,
    description text,
    organization_id int
);

CREATE TABLE DEVICE_DEVICE (
    id int PRIMARY KEY,
    identifier varchar,
    name varchar,
    mqtt_user varchar,
    mqtt_password varchar,
    is_active boolean,
    organization_id int
);

CREATE TABLE IAM_PERMISSION (
    id int PRIMARY KEY,
    operation varchar,
    name varchar,
    resource_type varchar
);

CREATE TABLE SENSOR_SENSOR (
    id int PRIMARY KEY,
    name varchar,
    description text,
    is_active boolean,
    unit varchar,
    measuring_type varchar,
    additional_info json,
    upper_limit int,
    lower_limit int,
    identifier varchar,
    device_id int
);

CREATE TABLE SENSOR_READING (
    sensor_uid uuid,
    timestamp datetime,
    value varchar,
    PRIMARY KEY (sensor_uid, timestamp)
);

CREATE TABLE IAM_ROLE_PERMISSIONS (
    id int PRIMARY KEY,
    role_id int,
    permission_id int
);

CREATE TABLE DASHBOARD_SENSORS (
    id int PRIMARY KEY,
    dashboard_id int,
    sensor_id int
);

CREATE TABLE IAM_USER_ROLES (
    user_id int,
    role_id int
);
 
ALTER TABLE IAM_USER ADD CONSTRAINT FK_IAM_USER_2
    FOREIGN KEY (organization_id)
    REFERENCES IAM_ORGANIZATION (id)
    ON DELETE CASCADE;
 
ALTER TABLE DASHBOARD_DASHBOARD ADD CONSTRAINT FK_DASHBOARD_DASHBOARD_2
    FOREIGN KEY (organization_id)
    REFERENCES IAM_ORGANIZATION (id)
    ON DELETE CASCADE;
 
ALTER TABLE DEVICE_DEVICE ADD CONSTRAINT FK_DEVICE_DEVICE_2
    FOREIGN KEY (organization_id)
    REFERENCES IAM_ORGANIZATION (id)
    ON DELETE CASCADE;
 
ALTER TABLE SENSOR_SENSOR ADD CONSTRAINT FK_SENSOR_SENSOR_2
    FOREIGN KEY (device_id)
    REFERENCES DEVICE_DEVICE (id)
    ON DELETE CASCADE;
 
ALTER TABLE IAM_ROLE_PERMISSIONS ADD CONSTRAINT FK_IAM_ROLE_PERMISSIONS_2
    FOREIGN KEY (role_id)
    REFERENCES IAM_ROLE (id);
 
ALTER TABLE IAM_ROLE_PERMISSIONS ADD CONSTRAINT FK_IAM_ROLE_PERMISSIONS_3
    FOREIGN KEY (permission_id)
    REFERENCES IAM_PERMISSION (id);
 
ALTER TABLE DASHBOARD_SENSORS ADD CONSTRAINT FK_DASHBOARD_SENSORS_2
    FOREIGN KEY (dashboard_id)
    REFERENCES DASHBOARD_DASHBOARD (id);
 
ALTER TABLE DASHBOARD_SENSORS ADD CONSTRAINT FK_DASHBOARD_SENSORS_3
    FOREIGN KEY (sensor_id)
    REFERENCES SENSOR_SENSOR (id);
 
ALTER TABLE IAM_USER_ROLES ADD CONSTRAINT FK_IAM_USER_ROLES_1
    FOREIGN KEY (user_id)
    REFERENCES IAM_USER (id)
    ON DELETE SET NULL;
 
ALTER TABLE IAM_USER_ROLES ADD CONSTRAINT FK_IAM_USER_ROLES_2
    FOREIGN KEY (role_id)
    REFERENCES IAM_ROLE (id)
    ON DELETE SET NULL;
