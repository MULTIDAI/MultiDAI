
# coding: utf-8

# In[ ]:

import SciServer
from SciServer import Authentication, LoginPortal, Config, CasJobs, SkyQuery, SciDrive, SkyServer, Files, Jobs
import os;
import pandas as pd;
import sys;
import json;
from io import StringIO
from io import BytesIO
#from PIL import Image
import numpy as np
import matplotlib.pyplot as plt



def createData(username, password, dec_low, dec_high, ra_low, ra_high, table_name, context, all_data):
    CasJobs_Database = "DR16"
    CasJobs_Query = "Select ra,dec into [myscratch:default]." + table_name + " from dr16.photoobjall "

    if(not all_data):
        CasJobs_Query += "where dec between " + str(dec_low) + " and " + str(dec_high) + " and ra between " + str(ra_low) + " and " + str(ra_high)

    login(username, password)
    
    jobId = CasJobs.submitJob(sql=CasJobs_Query, context=context)
    jobDescription = CasJobs.waitForJob(jobId=jobId, verbose=True)
    print("Data has been saved on sciserver at context myscratch with tablename " + table_name)

def createQueries(username, password, amount, table_name, context):
    CasJobs_Query = "SELECT top " + str(amount) + " statement into [myscratch:default]." + table_name + " FROM sdssweblogs.SqlLog WHERE CHARINDEX('FROM PhotoObjAll', statement) > 0 AND CHARINDEX('p.dec BETWEEN', statement) > 0 AND CHARINDEX('p.ra BETWEEN', statement) > 0 AND access='Skyserver.Search.SQL'" 
    login(username, password)
    
    jobId = CasJobs.submitJob(sql=CasJobs_Query, context=context)
    jobDescription = CasJobs.waitForJob(jobId=jobId, verbose=True)
    print("Data has been saved on sciserver at context myscratch with tablename " + table_name)

def login(username, password):
    token1 = Authentication.login(username, password);
    user = Authentication.getKeystoneUserWithToken(token1)
    print("userName=" + user.userName)
    print("id=" + user.id)
    iden = Authentication.identArgIdentifier()
    print("ident="+iden)


def downloadFile(username, password, tableName_, databaseName_, filename):
    login(username, password)
    print("Authentication complete, downloading table")
    table = SkyQuery.getTable(tableName=tableName_, datasetName=databaseName_)
    print("Download complete, writing to CSV")
    table.to_csv(filename)
    print("Table contents have been written to file " + filename)
    print("Table info: \n", table)
    # print(Files.getFileServices())


# print(jobId)
# print(jobDescription)

# tables = CasJobs.getTables(context="MyDB")
# print(tables)


# casJobsId = CasJobs.getSchemaName()
# print(casJobsId)

# jobId1 = CasJobs.submitJob(sql=CasJobs_QueriesQuery, context=CasJobs_Database)
# jobDescription1 = CasJobs.waitForJob(jobId=jobId1, verbose=True)
# print("Done saving queries")
# print(jobId1)
# print(jobDescription1)

# tables = CasJobs.getTables(context="MyDB")
# print(tables)

# print(Files.getFileServices(true))

#submit a job, which inserts the query results into a table in the MyDB database context. 
#Wait until the job is done and get its status.

# jobId = CasJobs.submitJob(sql=CasJobs_TestQuery + " into MyDB." + CasJobs_TestTableName1, context="MyDB")
# jobDescription = CasJobs.waitForJob(jobId=jobId, verbose=False)
# print("Done");
# print(jobId)
# print(jobDescription)

if __name__ == "__main__":
    username = '***'
    password = '***'
    # createData(username, password, 0, 1, 0, 1, 'data_test_newfunc1', 'DR16', False)
    # createQueries(username, password, 100000, 'queries_test_new_func1', 'DR16')
    downloadFile(username, password, "data_test_newfunc1", "myscratch", "data_test_newfunc.csv")
    downloadFile(username, password, "queries_test_new_func1", "myscratch", "queries_test_new_func.csv")
