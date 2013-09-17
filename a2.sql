--User Defined Types
create type AXE_Record as ("Date" date, Code char(3), Volume integer);
create type Sector_Avg as (Sector text, AvgRating numeric);
create type Relax_Company as (Code char(3), Name text, Address text, Zip varchar(10), Sector text);
create type Stock_Overall_Gain as (Code char(3), BeginPrice numeric, EndPrice numeric, Change numeric, Gain numeric);
create type Stock_Info as (Code char(3), MinPrice numeric, AvgPrice numeric, MaxPrice numeric, MinDayGain numeric, AvgDayGain numeric, MaxDayGain numeric);
--END User Defined Types

--User Defined Functions
create or replace function active_stock() returns setof AXE_Record
as $$
declare
	r record;
begin
	for r in select "Date" as d, max(volume) as maxvol from ASX group by "Date" order by "Date"
	loop
		 return query (select "Date", Code, Volume from ASX where "Date" = r.d and volume = r.maxvol); 
	end loop;
end;
$$ language plpgsql;

create or replace function sector_score() returns setof Sector_Avg
as $$
declare
	r record;
	ans Sector_Avg;
begin
	for r in select distinct sector from category
	loop
		ans.Sector = r.sector;
		ans.AvgRating := (select cast(avg(Star) as decimal(10,2)) from rating where code in (select code from category where sector = r.sector));
		return next ans; 
	end loop;
end;
$$ language plpgsql;

create or replace function competitor_free_company() returns setof Relax_Company
as $$
declare
	r record;
	t record;
	aus_comp_flag integer := 0;
	oversea_comp_flag integer := 0;
	ans Relax_Company;
begin
	for r in select distinct sector from category
	loop
		for t in select code from category where sector = r.sector
		loop
			if ((select country from Company where code = t.code) <> 'Australia') then
				oversea_comp_flag := 1;
				EXIT;
			else
				aus_comp_flag := 1;
			end if;	 
		end loop;
		if (oversea_comp_flag = 0) then
			if (aus_comp_flag = 1) then
				aus_comp_flag := 0;
				for t in select code from category where sector = r.sector
				loop
					ans.Code = t.code;
					ans.Name = (select name from company where code = t.code);
					ans.Address = (select address from company where code = t.code);
					ans.Zip = (select zip from company where code = t.code);
					ans.Sector = r.sector;
					return next ans; 
				end loop;
			end if;
		else
			aus_comp_flag = 0;
			oversea_comp_flag = 0;
		end if; 
	end loop;
end;
$$ language plpgsql;

create or replace function stock_gain_summary() returns setof Stock_Overall_Gain
as $$ 
declare 
	r record;
	ans Stock_Overall_Gain;
begin
	for r in select distinct code from asx
	loop
		ans.Code = r.code;
		ans.BeginPrice = (select price from asx where "Date" = (select min("Date") from asx where code = r.code) and code = r.code);
		ans.EndPrice = (select price from asx where "Date" = (select max("Date") from asx where code = r.code) and code = r.code);
		ans.Change = ans.EndPrice - ans.BeginPrice;
		ans.Gain = (cast(((ans.Change / ans.BeginPrice) * 100) as decimal(10,2)));
		return next ans;
	end loop;
end;
$$ language plpgsql;

create or replace function stock_info_summary() returns setof Stock_Info
as $$
declare 
	each_code record;
	total_dates integer := 0;
	total_price numeric := 0;
	total_gain numeric := 0;
	ans Stock_Info;
begin
	for each_code in select distinct code from Q7
	loop
		total_dates = (select count(*) from Q7 where code = each_code.code);
		total_price = (select sum(price) from asx where code = each_code.code);
		total_gain = (select sum(gain) from Q7 where code = each_code.code);
		ans.Code = each_code.code;
		ans.MinPrice = (select min(price) from asx where code = each_code.code);
		ans.MaxPrice = (select max(price) from asx where code = each_code.code);
		ans.MinDayGain = (select min(gain) from Q7 where code = each_code.code);
		ans.MaxDayGain = (select max(gain) from Q7 where code = each_code.code);
		ans.AvgPrice = (cast((total_price / (total_dates + 1)) as decimal(10,2)));
		ans.AvgDayGain = (cast((total_gain / total_dates) as decimal(10,2)));
		return next ans;
	end loop;
end;
$$ language plpgsql;
--END User Defined Functions

--Views
create or replace view Q1(Name, Country) as select Name, Country from Company where country <> 'Australia';
create or replace view Q2(Code) as select code from (select code, count(*) from executive group by code having count(*) > 5) as a;
create or replace view Q3(Name) as select name from Company where code in (select code from Category where sector = 'Technology');
create or replace view Q4(Sector, Number) as select sector, count(*) as number from Category group by sector;
create or replace view Q5(Name) as select distinct person from executive where code in (select code from company where name in (select * from Q3));
create or replace view Q6(Name) as select name from company where code in (select code from category where sector = 'Services') and country = 'Australia' and zip like '2%';
create or replace view Q7("Date", Code, Volume, PrevPrice, Price, Change, Gain) as select "Date", code, volume, prevprice, price, (price - prevprice) as Change, cast((((price - prevprice) / prevprice) * 100) as decimal(10,2)) as Gain from (select *, lag(price) over (partition by code order by "Date") as prevprice from ASX) as a where a.prevprice is not null;
create or replace view Q8("Date", Code, Volume) as select * from active_stock() order by "Date", code;
create or replace view Q9(Sector, Industry, Number) as select sector, industry, count(*) from Category group by industry, sector order by sector, industry;
create or replace view Q10(Code, Industry) as select code, industry from Category where industry in (select industry from Q9 where number = 1);
create or replace view Q11(Sector, AvgRating) as select * from sector_score() order by AvgRating desc;
create or replace view Q12(Name) as select person from (select person, count(*) from executive group by person having count(*) > 1) as a;
create or replace view Q13(Code, Name, Address, Zip, Sector) as select * from competitor_free_company();
create or replace view Q14(Code, BeginPrice, EndPrice, Change, Gain) as select * from stock_gain_summary() order by gain desc, code asc;
create or replace view Q15(Code, MinPrice, AvgPrice, MaxPrice, MinDayGain, AvgDayGain, MaxDayGain) as select * from stock_info_summary();
--END Views

--Triggers
--END Triggers



