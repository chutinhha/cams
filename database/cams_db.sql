
--CREATE TABLE CAMS_Main (P_Id int, Category varchar(255), FirstName varchar(255), Address varchar(255),	City varchar(255))

--INSERT INTO Persons VALUES (1,'Gupta', 'Saurabh', 'Lucknow', 'Uttar Pradeshs')

--SELECT * FROM Persons

--select * from tblCatalog WHERE Manufacturer='Lenovo'

/*
SELECT Distinct City FROM Persons
SELECT * FROM Persons WHERE City='kanpur' AND LastName='Gupta'
SELECT * FROM Persons WHERE City='kanpur' OR LastName='Gupta'
SELECT  City FROM Persons ORDER BY Address ASC
*/
/*
delete from Persons where P_Id='5'
*/


use cams

----tbluserdetails------

select * from tbluserdetails
select * from tbluserdetails where id=1

select UserName from tbluserdetails where id=1
insert into tbluserdetails values ('support', '+915222702902', 'support@ccplusplus.com', 1, 'support', 'cams support team', 1, 0, 1)
update tbluserdetails set Email='cams@ccplusplus.com' where id=1
update tbluserdetails set Password='cams' where UserName='Sahil'
update tbluserdetails set Contact='0011223344' where id=2
update tbluserdetails set Note='default user' where id=1
update tbluserdetails set Note='registered user' where id=2

select * from gen_tblMastertable

select * from gen_tblMastertable

select * from gen_tblMastertablevalues

select * from TblItemGroupMaster

select * from tblSoftwareProDetails

select * from tblCatalog

-----tblAssetDetails----
select * from tblAssetDetails
insert into tblAssetDetails values ('001', 'Laptop', 01, 01, 01, '01', 'laptop product', 01, 'Lenovo', '1111', '1111', '2012-10-01', '2013-10-01', '2012-10-06', 5000.5, 1000, 0)
select * from tblAssetDetails where AssetCode='001'
delete from tblAssetDetails where id=1


