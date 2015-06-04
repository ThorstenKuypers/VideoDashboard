SELECT cust.customer_id,name,mail,test,machineID FROM cust
JOIN mid ON(cust.customer_id=mid.customer_id)
JOIN test ON(cust.customer_id=test.customer_id)
WHERE cust.customer_id=123;
