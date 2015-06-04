CREATE TABLE cust
(
	customer_id INT PRIMARY KEY,
	name VARCHAR(20),
	mail VARCHAR(20)
);

CREATE TABLE mid
(
	id INT IDENTITY(1,1) PRIMARY KEY,
	customer_id INT,
	machineID VARCHAR(20)
	FOREIGN KEY (customer_id) REFERENCES cust(customer_id)
);