using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using System.Windows.Forms;
using System.Xml;
using DocumentFormat.OpenXml;
using DocumentFormat.OpenXml.Packaging;
using DocumentFormat.OpenXml.Spreadsheet;
using System.Data;
using System.Text.RegularExpressions;

namespace SDARI_Hvac_Report
{
    class Program
    {
        static void Main(string[] args)
        {
            //读取命令行参数，并检查其完整性
            string TemplateFile = args[0];
            string DataFile = args[1];
            string TargetFile = args[2];
            if (TemplateFile.Length == 0 | DataFile.Length == 0 | TargetFile.Length == 0){
                MessageBox.Show("参数不完整，无法生成报表！", "错误", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            //读取报表数据文件
            string[] Data;
            try{
                Data = (string[])File.ReadAllLines(DataFile, Encoding.Default);
            }
            catch{
                MessageBox.Show("读取数据文件时出错，无法生成报表！", "错误", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }


            //检查目标文件是否已经存在，如果存在则删除，然后将模板文件拷贝到目标文件
            try{
                File.Copy(TemplateFile, TargetFile);
            }
            catch{
                try{
                    File.Delete(TargetFile);
                }
                catch{
                    MessageBox.Show("文件被占用，请关闭文件后再重新运行这个程序", "错误", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return;
                }
                File.Copy(TemplateFile, TargetFile);
            }

            //获取报表名称
            string[] seperator = { ",#" };
            string reportInformation = Data[0];
            string[] reportInformations = (string[])reportInformation.Split(seperator, StringSplitOptions.None);
            string reportName = reportInformations[0];

            //根据报表名称调用对应的方法处理
            if (reportName == "风管内场托盘表"){
                HvacReport(Data, TargetFile);
            }
            else if (reportName == "风管安装托盘表"){
                HvacInstallationReport(Data, TargetFile);
            }
            else if (reportName == "风管支架托盘表"){
                HvacSupportReport(Data, TargetFile);
            }
            else if (reportName == "风管支架安装托盘表"){
                HvacSupportInstallationReport(Data, TargetFile);
            }
            else if (reportName == "挡水圈护圈清册"){
                PenetrationListReport(Data, TargetFile);
            }
            else if (reportName == "管子开孔表"){
                PipeHoleListReport(Data, TargetFile);
            }

            //打开报表文件
            try{
                System.Diagnostics.Process.Start(TargetFile);
            }
            catch{
                return;
            }
        }

        public static void AppendLineToDataTable(string[] elements, DataTable table)
        {
            DataRow dr = table.NewRow();
            int length = elements.Length;
            string[] datas = new string[length-2];
            Array.Copy(elements, 1, datas, 0,length - 2);
            dr.ItemArray = datas;
            table.Rows.Add(dr);
        }

       //风管内场托盘表
        public static void HvacReport(string[] datas, string report) 
        {
            string[] seperator = { ",#" };                       //分隔符

            //定义保存报表数据的数据表（Sheet）
            DataTable hvac_SpoolList = new DataTable();          //风管内场托盘表

            //用于保存报表的抬头位置
            int[,] subTitleInfo = new int[5, 2];
            DataTable hvac_TitleList = new DataTable();          //托盘表信息  
            subTitleInfo[0, 0] = 3;//工程号
            subTitleInfo[0, 1] = 1;//工程号
            subTitleInfo[1, 0] = 2;//工程名称
            subTitleInfo[1, 1] = 1;//工程名称
            subTitleInfo[2, 0] = 3;//区域
            subTitleInfo[2, 1] = 3;//区域
            subTitleInfo[3, 0] = 3;//托盘号
            subTitleInfo[3, 1] = 4;//托盘号
            subTitleInfo[4, 0] = 2;//图号
            subTitleInfo[4, 1] = 5;//图号

            string subReport = "";
            int[,] subReportInfo = new int[3,3];                 //用于保存报表的起始行及起始列及总列数
            foreach (string line in datas){
                string[] elements = (string[])line.Split(seperator, StringSplitOptions.None);
                string element0 = elements[0];
                if (element0 == "Table"){//初始化数据表
                    subReport = elements[1];
                    int columnNumber = Convert.ToInt32(elements[4]);
                    if (subReport == "风管内场托盘表"){
                        int startRow = Convert.ToInt32(elements[2]);
                        int startColumn = Convert.ToInt32(elements[3]);
                        int totalColumn = Convert.ToInt32(elements[4]);
                        subReportInfo[0, 0] = startRow;
                        subReportInfo[0, 1] = startColumn;
                        subReportInfo[0, 2] = totalColumn;
                        if (hvac_SpoolList.Columns.Count <= 0){
                            for (int i = 0; i < columnNumber; i++){
                                hvac_SpoolList.Columns.Add(i.ToString(), typeof(string));
                            }
                        }
                    }
                }
                else if (element0 == "Line"){ //将报表数据保存到数据表中
                    if (subReport == "风管内场托盘表"){
                        AppendLineToDataTable(elements, hvac_SpoolList);
                    }
                }
                else if (element0 == "风管内场托盘表"){ //将表头数据保存到数据表中
                    if (hvac_SpoolList.Columns.Count <= 0){
                        for (int i = 0; i < subTitleInfo.GetLength(0); i++){
                            hvac_TitleList.Columns.Add(i.ToString(), typeof(string));
                        }
                    }
                    AppendLineToDataTable(elements, hvac_TitleList);
                }
            }

            //对报表进行排序
            //if (hvac_SpoolList.Rows.Count > 1){
            //    hvac_SpoolList.DefaultView.Sort = "1";
            //    hvac_SpoolList = hvac_SpoolList.DefaultView.ToTable();
            //    for (int i = 0; i < hvac_SpoolList.Rows.Count; i++){
            //        hvac_SpoolList.Rows[i][0] = Convert.ToString(i + 1);
            //    }
            //}

            // 检查报表模板
            List<string> sheets = GetSheets(report);

            // 写数据到《风管内场托盘表》
            Boolean exist = sheets.Contains("风管内场托盘表");
            if (exist){
                uint total_row = Convert.ToUInt32(hvac_SpoolList.Rows.Count);
                uint total_column = Convert.ToUInt32(subReportInfo[0, 2]);
                uint start_row = Convert.ToUInt32(subReportInfo[0, 0]);
                uint start_column = Convert.ToUInt32(subReportInfo[0, 1]);
                if (total_row > 0){
                    WriteDataToReport("风管内场托盘表", hvac_SpoolList, report, total_row, total_column, start_row, start_column);
                    WriteTitleToReport("风管内场托盘表", hvac_TitleList, report, subTitleInfo);
                }
            }
            else{
                MessageBox.Show("模板文件中没有《风管内场托盘表》数据页，无法生成这个报表", "错误", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
            return;
        }

        //风管安装托盘表
        public static void HvacInstallationReport(string[] datas, string report)
        {
            string[] seperator = { ",#" };                                   //分隔符

            //定义保存报表数据的数据表（Sheet）
            DataTable hvac_SpoolInstallationList = new DataTable();          //风管安装托盘表
            DataTable hvac_ValveInstallationList = new DataTable();          //风管附件安装托盘表

            //用于保存报表的抬头位置
            int[,] subTitleInfo = new int[5, 2];
            DataTable hvac_TitleList = new DataTable();                     //托盘表信息  
            subTitleInfo[0, 0] = 3;//工程号
            subTitleInfo[0, 1] = 1;//工程号
            subTitleInfo[1, 0] = 2;//工程名称
            subTitleInfo[1, 1] = 1;//工程名称
            subTitleInfo[2, 0] = 3;//区域
            subTitleInfo[2, 1] = 3;//区域
            subTitleInfo[3, 0] = 3;//托盘号
            subTitleInfo[3, 1] = 4;//托盘号
            subTitleInfo[4, 0] = 2;//图号
            subTitleInfo[4, 1] = 5;//图号

            string subReport = "";
            int[,] subReportInfo = new int[3, 3];                 //用于保存报表的起始行及起始列及总列数
            foreach (string line in datas){
                string[] elements = (string[])line.Split(seperator, StringSplitOptions.None);
                string element0 = elements[0];
                if (element0 == "Table"){//初始化数据表
                    subReport = elements[1];
                    int columnNumber = Convert.ToInt32(elements[4]);
                    if (subReport == "风管安装托盘表"){
                        int startRow = Convert.ToInt32(elements[2]);
                        int startColumn = Convert.ToInt32(elements[3]);
                        int totalColumn = Convert.ToInt32(elements[4]);
                        subReportInfo[0, 0] = startRow;
                        subReportInfo[0, 1] = startColumn;
                        subReportInfo[0, 2] = totalColumn;
                        if (hvac_SpoolInstallationList.Columns.Count <= 0){
                            for (int i = 0; i < columnNumber; i++){
                                hvac_SpoolInstallationList.Columns.Add(i.ToString(), typeof(string));
                            }
                        }
                    }
                    else if (subReport == "风管附件安装托盘表"){
                        int startRow = Convert.ToInt32(elements[2]);
                        int startColumn = Convert.ToInt32(elements[3]);
                        int totalColumn = Convert.ToInt32(elements[4]);
                        subReportInfo[0, 0] = startRow;
                        subReportInfo[0, 1] = startColumn;
                        subReportInfo[0, 2] = totalColumn;
                        if (hvac_ValveInstallationList.Columns.Count <= 0){
                            for (int i = 0; i < columnNumber; i++){
                                hvac_ValveInstallationList.Columns.Add(i.ToString(), typeof(string));
                            }
                        }
                    }
                }
                else if (element0 == "Line"){ //将报表数据保存到数据表中
                    if (subReport == "风管安装托盘表"){
                        AppendLineToDataTable(elements, hvac_SpoolInstallationList);
                    }
                    else if (subReport == "风管附件安装托盘表"){
                        AppendLineToDataTable(elements, hvac_ValveInstallationList);
                    }
                }
                else{//将表头数据保存到数据表中
                    if (element0 == "风管安装托盘表"){ 
                        if (hvac_SpoolInstallationList.Columns.Count <= 0) {
                            for (int i = 0; i < subTitleInfo.GetLength(0); i++)
                            {
                                hvac_TitleList.Columns.Add(i.ToString(), typeof(string));
                            }
                        }
                        AppendLineToDataTable(elements, hvac_TitleList);
                    }
                    else if (element0 == "风管附件安装托盘表"){
                        if (hvac_ValveInstallationList.Columns.Count <= 0){
                            for (int i = 0; i < subTitleInfo.GetLength(0); i++){
                                hvac_TitleList.Columns.Add(i.ToString(), typeof(string));
                            }
                        }
                        AppendLineToDataTable(elements, hvac_TitleList);
                    }
                }
            }

            //对报表进行排序
            if (hvac_SpoolInstallationList.Rows.Count > 1){
                hvac_SpoolInstallationList.DefaultView.Sort = "1";
                hvac_SpoolInstallationList = hvac_SpoolInstallationList.DefaultView.ToTable();
                for (int i = 0; i < hvac_SpoolInstallationList.Rows.Count; i++){
                    hvac_SpoolInstallationList.Rows[i][0] = Convert.ToString(i + 1);
                }
            }
            if (hvac_ValveInstallationList.Rows.Count > 1){
                hvac_ValveInstallationList.DefaultView.Sort = "2";
                hvac_ValveInstallationList = hvac_ValveInstallationList.DefaultView.ToTable();
                for (int i = 0; i < hvac_ValveInstallationList.Rows.Count; i++){
                    hvac_ValveInstallationList.Rows[i][0] = Convert.ToString(i + 1);
                }
            }

            // 检查报表模板
            List<string> sheets = GetSheets(report);

            // 写数据到《风管安装托盘表》
            Boolean exist1 = sheets.Contains("风管安装托盘表");
            if (exist1){
                uint total_row = Convert.ToUInt32(hvac_SpoolInstallationList.Rows.Count);
                uint total_column = Convert.ToUInt32(subReportInfo[0, 2]);
                uint start_row = Convert.ToUInt32(subReportInfo[0, 0]);
                uint start_column = Convert.ToUInt32(subReportInfo[0, 1]);
                if (total_row > 0){
                    WriteDataToReport("风管安装托盘表", hvac_SpoolInstallationList, report, total_row, total_column, start_row, start_column);
                    WriteTitleToReport("风管安装托盘表", hvac_TitleList, report, subTitleInfo);
                }
            }
            else{
                MessageBox.Show("模板文件中没有《风管安装托盘表》数据页，无法生成这个报表", "错误", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
            // 写数据到《风管附件安装托盘表》
            Boolean exist2 = sheets.Contains("风管附件安装托盘表");
            if (exist2){
                uint total_row = Convert.ToUInt32(hvac_ValveInstallationList.Rows.Count);
                uint total_column = Convert.ToUInt32(subReportInfo[0, 2]);
                uint start_row = Convert.ToUInt32(subReportInfo[0, 0]);
                uint start_column = Convert.ToUInt32(subReportInfo[0, 1]);
                if (total_row > 0){
                    WriteDataToReport("风管附件安装托盘表", hvac_ValveInstallationList, report, total_row, total_column, start_row, start_column);
                    WriteTitleToReport("风管附件安装托盘表", hvac_TitleList, report, subTitleInfo);
                }
            }
            else{
                MessageBox.Show("模板文件中没有《风管附件安装托盘表》数据页，无法生成这个报表", "错误", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
            return;
        }

        //风管支架托盘表
        public static void HvacSupportReport(string[] datas, string report)
        {
            string[] seperator = { ",#" };                        //分隔符

            //定义保存报表数据的数据表（Sheet）
            DataTable hvac_SupportList = new DataTable();         //风管内场托盘表

            //用于保存报表的抬头位置
            int[,] subTitleInfo = new int[5, 2];
            DataTable hvac_TitleList = new DataTable();          //托盘表信息  
            subTitleInfo[0, 0] = 3;//工程号
            subTitleInfo[0, 1] = 1;//工程号
            subTitleInfo[1, 0] = 2;//工程名称
            subTitleInfo[1, 1] = 1;//工程名称
            subTitleInfo[2, 0] = 3;//区域
            subTitleInfo[2, 1] = 3;//区域
            subTitleInfo[3, 0] = 3;//托盘号
            subTitleInfo[3, 1] = 4;//托盘号
            subTitleInfo[4, 0] = 2;//图号
            subTitleInfo[4, 1] = 5;//图号

            string subReport = "";
            int[,] subReportInfo = new int[3, 3];                 //用于保存报表的起始行及起始列及总列数
            foreach (string line in datas){
                string[] elements = (string[])line.Split(seperator, StringSplitOptions.None);
                string element0 = elements[0];
                if (element0 == "Table"){//初始化数据表
                    subReport = elements[1];
                    int columnNumber = Convert.ToInt32(elements[4]);
                    if (subReport == "风管支架托盘表"){
                        int startRow = Convert.ToInt32(elements[2]);
                        int startColumn = Convert.ToInt32(elements[3]);
                        int totalColumn = Convert.ToInt32(elements[4]);
                        subReportInfo[0, 0] = startRow;
                        subReportInfo[0, 1] = startColumn;
                        subReportInfo[0, 2] = totalColumn;
                        if (hvac_SupportList.Columns.Count <= 0){
                            for (int i = 0; i < columnNumber; i++){
                                hvac_SupportList.Columns.Add(i.ToString(), typeof(string));
                            }
                        }
                    }
                }
                else if (element0 == "Line"){ //将报表数据保存到数据表中
                    if (subReport == "风管支架托盘表"){
                        AppendLineToDataTable(elements, hvac_SupportList);
                    }
                }
                else if (element0 == "风管支架托盘表"){ //将表头数据保存到数据表中
                    if (hvac_SupportList.Columns.Count <= 0){
                        for (int i = 0; i < subTitleInfo.GetLength(0); i++){
                            hvac_TitleList.Columns.Add(i.ToString(), typeof(string));
                        }
                    }
                    AppendLineToDataTable(elements, hvac_TitleList);
                }
            }

            //对报表进行排序
            //if (hvac_SupportList.Rows.Count > 1){
            //    hvac_SupportList.DefaultView.Sort = "1";
            //    hvac_SupportList = hvac_SupportList.DefaultView.ToTable();
            //    for (int i = 0; i < hvac_SupportList.Rows.Count; i++){
            //        hvac_SupportList.Rows[i][0] = Convert.ToString(i + 1);
            //    }
            //}

            // 检查报表模板
            List<string> sheets = GetSheets(report);

            // 写数据到《风管支架托盘表》
            Boolean exist = sheets.Contains("风管支架托盘表");
            if (exist){
                uint total_row = Convert.ToUInt32(hvac_SupportList.Rows.Count);
                uint total_column = Convert.ToUInt32(subReportInfo[0, 2]);
                uint start_row = Convert.ToUInt32(subReportInfo[0, 0]);
                uint start_column = Convert.ToUInt32(subReportInfo[0, 1]);
                if (total_row > 0){
                    WriteDataToReport("风管支架托盘表", hvac_SupportList, report, total_row, total_column, start_row, start_column);
                    WriteTitleToReport("风管支架托盘表", hvac_TitleList, report, subTitleInfo);
                }
            }
            else{
                MessageBox.Show("模板文件中没有《风管支架托盘表》数据页，无法生成这个报表", "错误", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
            return;
        }

        //风管支架安装托盘表
        public static void HvacSupportInstallationReport(string[] datas, string report)
        {
            string[] seperator = { ",#" };                            //分隔符

            //定义保存报表数据的数据表（Sheet）
            DataTable hvac_SupportInstallationList = new DataTable(); //风管支架安装托盘表

            //用于保存报表的抬头位置
            int[,] subTitleInfo = new int[5, 2];
            DataTable hvac_TitleList = new DataTable();               //托盘表信息  
            subTitleInfo[0, 0] = 3;//工程号
            subTitleInfo[0, 1] = 1;//工程号
            subTitleInfo[1, 0] = 2;//工程名称
            subTitleInfo[1, 1] = 1;//工程名称
            subTitleInfo[2, 0] = 3;//区域
            subTitleInfo[2, 1] = 3;//区域
            subTitleInfo[3, 0] = 3;//托盘号
            subTitleInfo[3, 1] = 4;//托盘号
            subTitleInfo[4, 0] = 2;//图号
            subTitleInfo[4, 1] = 5;//图号

            string subReport = "";
            int[,] subReportInfo = new int[3, 3];                 //用于保存报表的起始行及起始列及总列数
            foreach (string line in datas){
                string[] elements = (string[])line.Split(seperator, StringSplitOptions.None);
                string element0 = elements[0];
                if (element0 == "Table"){//初始化数据表
                    subReport = elements[1];
                    int columnNumber = Convert.ToInt32(elements[4]);
                    if (subReport == "风管支架安装托盘表"){
                        int startRow = Convert.ToInt32(elements[2]);
                        int startColumn = Convert.ToInt32(elements[3]);
                        int totalColumn = Convert.ToInt32(elements[4]);
                        subReportInfo[0, 0] = startRow;
                        subReportInfo[0, 1] = startColumn;
                        subReportInfo[0, 2] = totalColumn;
                        if (hvac_SupportInstallationList.Columns.Count <= 0){
                            for (int i = 0; i < columnNumber; i++){
                                hvac_SupportInstallationList.Columns.Add(i.ToString(), typeof(string));
                            }
                        }
                    }
                }
                else if (element0 == "Line"){ //将报表数据保存到数据表中
                    if (subReport == "风管支架安装托盘表"){
                        AppendLineToDataTable(elements, hvac_SupportInstallationList);
                    }
                }
                else if (element0 == "风管支架安装托盘表"){ //将表头数据保存到数据表中
                    if (hvac_SupportInstallationList.Columns.Count <= 0){
                        for (int i = 0; i < subTitleInfo.GetLength(0); i++){
                            hvac_TitleList.Columns.Add(i.ToString(), typeof(string));
                        }
                    }
                    AppendLineToDataTable(elements, hvac_TitleList);
                }
            }

            //对报表进行排序
            if (hvac_SupportInstallationList.Rows.Count > 1){
                hvac_SupportInstallationList.DefaultView.Sort = "1";
                hvac_SupportInstallationList = hvac_SupportInstallationList.DefaultView.ToTable();
                for (int i = 0; i < hvac_SupportInstallationList.Rows.Count; i++){
                    hvac_SupportInstallationList.Rows[i][0] = Convert.ToString(i + 1);
                }
            }

            // 检查报表模板
            List<string> sheets = GetSheets(report);

            // 写数据到《风管支架安装托盘表》
            Boolean exist = sheets.Contains("风管支架安装托盘表");
            if (exist){
                uint total_row = Convert.ToUInt32(hvac_SupportInstallationList.Rows.Count);
                uint total_column = Convert.ToUInt32(subReportInfo[0, 2]);
                uint start_row = Convert.ToUInt32(subReportInfo[0, 0]);
                uint start_column = Convert.ToUInt32(subReportInfo[0, 1]);
                if (total_row > 0){
                    WriteDataToReport("风管支架安装托盘表", hvac_SupportInstallationList, report, total_row, total_column, start_row, start_column);
                    WriteTitleToReport("风管支架安装托盘表", hvac_TitleList, report, subTitleInfo);
                }
            }
            else{
                MessageBox.Show("模板文件中没有《风管支架安装托盘表》数据页，无法生成这个报表", "错误", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
            return;
        }

        //挡水圈清册
        public static void PenetrationListReport(string[] datas, string report)
        {
            string[] seperator = { ",#" };                            //分隔符

            //定义保存报表数据的数据表（Sheet）
            DataTable penetrationList = new DataTable();              //挡水圈护圈清册
            DataTable penetrationMatrialList = new DataTable();       //挡水圈护圈材料汇总

            //用于保存报表的抬头位置
            int[,] subTitleInfo = new int[5, 2];
            DataTable pent_TitleList = new DataTable();               //托盘表信息  
            subTitleInfo[0, 0] = 3;//工程号
            subTitleInfo[0, 1] = 1;//工程号
            subTitleInfo[1, 0] = 2;//工程名称
            subTitleInfo[1, 1] = 1;//工程名称
            subTitleInfo[2, 0] = 3;//区域
            subTitleInfo[2, 1] = 3;//区域
            subTitleInfo[3, 0] = 3;//托盘号
            subTitleInfo[3, 1] = 4;//托盘号
            subTitleInfo[4, 0] = 2;//图号
            subTitleInfo[4, 1] = 5;//图号

            string subReport = "";
            int[,] subReportInfo = new int[3, 3];                 //用于保存报表的起始行及起始列及总列数
            foreach (string line in datas){
                string[] elements = (string[])line.Split(seperator, StringSplitOptions.None);
                string element0 = elements[0];
                if (element0 == "Table"){//初始化数据表
                    subReport = elements[1];
                    int columnNumber = Convert.ToInt32(elements[4]);
                    if (subReport == "挡水圈护圈清册"){
                        int startRow = Convert.ToInt32(elements[2]);
                        int startColumn = Convert.ToInt32(elements[3]);
                        int totalColumn = Convert.ToInt32(elements[4]);
                        subReportInfo[0, 0] = startRow;
                        subReportInfo[0, 1] = startColumn;
                        subReportInfo[0, 2] = totalColumn;
                        if (penetrationList.Columns.Count <= 0){
                            for (int i = 0; i < columnNumber; i++){
                                penetrationList.Columns.Add(i.ToString(), typeof(string));
                            }
                        }
                    }
                    else if (subReport == "挡水圈护圈材料汇总"){
                        int startRow = Convert.ToInt32(elements[2]);
                        int startColumn = Convert.ToInt32(elements[3]);
                        int totalColumn = Convert.ToInt32(elements[4]);
                        subReportInfo[0, 0] = startRow;
                        subReportInfo[0, 1] = startColumn;
                        subReportInfo[0, 2] = totalColumn;
                        if (penetrationMatrialList.Columns.Count <= 0){
                            for (int i = 0; i < columnNumber; i++){
                                penetrationMatrialList.Columns.Add(i.ToString(), typeof(string));
                            }
                        }
                    }
                }
                else if (element0 == "Line"){ //将报表数据保存到数据表中
                    if (subReport == "挡水圈护圈清册"){
                        AppendLineToDataTable(elements, penetrationList);
                    }
                    else if (subReport == "挡水圈护圈材料汇总"){
                        AppendLineToDataTable(elements, penetrationMatrialList);
                    }
                }
                else if (element0 == "挡水圈护圈清册"){ //将表头数据保存到数据表中
                    if (penetrationList.Columns.Count <= 0){
                        for (int i = 0; i < subTitleInfo.GetLength(0); i++){
                            pent_TitleList.Columns.Add(i.ToString(), typeof(string));
                        }
                    }
                    AppendLineToDataTable(elements, pent_TitleList);
                }
                else if (element0 == "挡水圈护圈材料汇总"){
                    if (penetrationMatrialList.Columns.Count <= 0){
                        for (int i = 0; i < subTitleInfo.GetLength(0); i++){
                            pent_TitleList.Columns.Add(i.ToString(), typeof(string));
                        }
                    }
                    AppendLineToDataTable(elements, pent_TitleList);
                }
            }

            //对报表进行排序
            if (penetrationList.Rows.Count > 1){
                penetrationList.DefaultView.Sort = "2";
                penetrationList = penetrationList.DefaultView.ToTable();
                for (int i = 0; i < penetrationList.Rows.Count; i++){
                    penetrationList.Rows[i][0] = Convert.ToString(i + 1);
                }
            }
            if (penetrationMatrialList.Rows.Count > 1){
                penetrationMatrialList.DefaultView.Sort = "1";
                penetrationMatrialList = penetrationMatrialList.DefaultView.ToTable();
                for (int i = 0; i < penetrationMatrialList.Rows.Count; i++){
                    penetrationMatrialList.Rows[i][0] = Convert.ToString(i + 1);
                }
            }

            // 检查报表模板
            List<string> sheets = GetSheets(report);

            // 写数据到《挡水圈护圈清册》
            Boolean exist = sheets.Contains("挡水圈护圈清册");
            if (exist){
                uint total_row = Convert.ToUInt32(penetrationList.Rows.Count);
                uint total_column = Convert.ToUInt32(subReportInfo[0, 2]);
                uint start_row = Convert.ToUInt32(subReportInfo[0, 0]);
                uint start_column = Convert.ToUInt32(subReportInfo[0, 1]);
                if (total_row > 0){
                    WriteDataToReport("挡水圈护圈清册", penetrationList, report, total_row, total_column, start_row, start_column);
                    WriteTitleToReport("挡水圈护圈清册", pent_TitleList, report, subTitleInfo);
                }
            }
            else{
                MessageBox.Show("模板文件中没有《挡水圈护圈清册》数据页，无法生成这个报表", "错误", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
            // 写数据到《挡水圈护圈材料汇总》
            Boolean exist2 = sheets.Contains("挡水圈护圈材料汇总");
            if (exist2){
                uint total_row = Convert.ToUInt32(penetrationMatrialList.Rows.Count);
                uint total_column = Convert.ToUInt32(subReportInfo[0, 2]);
                uint start_row = Convert.ToUInt32(subReportInfo[0, 0]);
                uint start_column = Convert.ToUInt32(subReportInfo[0, 1]);
                if (total_row > 0){
                    WriteDataToReport("挡水圈护圈材料汇总", penetrationMatrialList, report, total_row, total_column, start_row, start_column);
                    WriteTitleToReport("挡水圈护圈材料汇总", pent_TitleList, report, subTitleInfo);
                }
            }
            else{
                MessageBox.Show("模板文件中没有《挡水圈护圈材料汇总》数据页，无法生成这个报表", "错误", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
            return;
        }

        //管子开孔表
        public static void PipeHoleListReport(string[] datas, string report)
        {
            string[] seperator = { ",#" };                       //分隔符

            //定义保存报表数据的数据表（Sheet）
            DataTable pipe_HoleList = new DataTable();           //管子开孔表

            //用于保存报表的抬头位置
            int[,] subTitleInfo = new int[5, 2];
            DataTable pipe_TitleList = new DataTable();          //托盘表信息  
            subTitleInfo[0, 0] = 2;//工程号
            subTitleInfo[0, 1] = 1;//工程号
            subTitleInfo[1, 0] = 2;//工程名称
            subTitleInfo[1, 1] = 1;//工程名称
            subTitleInfo[2, 0] = 2;//区域
            subTitleInfo[2, 1] = 4;//区域
            subTitleInfo[3, 0] = 2;//托盘号
            subTitleInfo[3, 1] = 7;//托盘号
            subTitleInfo[4, 0] = 2;//图号
            subTitleInfo[4, 1] = 10;//图号

            string subReport = "";
            int[,] subReportInfo = new int[3, 3];                 //用于保存报表的起始行及起始列及总列数
            foreach (string line in datas){
                string[] elements = (string[])line.Split(seperator, StringSplitOptions.None);
                string element0 = elements[0];
                if (element0 == "Table"){//初始化数据表
                    subReport = elements[1];
                    int columnNumber = Convert.ToInt32(elements[4]);
                    if (subReport == "管子开孔表"){
                        int startRow = Convert.ToInt32(elements[2]);
                        int startColumn = Convert.ToInt32(elements[3]);
                        int totalColumn = Convert.ToInt32(elements[4]);
                        subReportInfo[0, 0] = startRow;
                        subReportInfo[0, 1] = startColumn;
                        subReportInfo[0, 2] = totalColumn;
                        if (pipe_HoleList.Columns.Count <= 0){
                            for (int i = 0; i < columnNumber; i++){
                                pipe_HoleList.Columns.Add(i.ToString(), typeof(string));
                            }
                        }
                    }
                }
                else if (element0 == "Line"){ //将报表数据保存到数据表中
                    if (subReport == "管子开孔表"){
                        AppendLineToDataTable(elements, pipe_HoleList);
                    }
                }
                else if (element0 == "管子开孔表"){ //将表头数据保存到数据表中
                    if (pipe_HoleList.Columns.Count <= 0){
                        for (int i = 0; i < subTitleInfo.GetLength(0); i++){
                            pipe_TitleList.Columns.Add(i.ToString(), typeof(string));
                        }
                    }
                    AppendLineToDataTable(elements, pipe_TitleList);
                }
            }

            //对报表进行排序
            if (pipe_HoleList.Rows.Count > 1){
                pipe_HoleList.DefaultView.Sort = "2";
                pipe_HoleList = pipe_HoleList.DefaultView.ToTable();
                for (int i = 0; i < pipe_HoleList.Rows.Count; i++){
                    pipe_HoleList.Rows[i][0] = Convert.ToString(i + 1);
                }
            }

            // 检查报表模板
            List<string> sheets = GetSheets(report);

            // 写数据到《管子开孔表》
            Boolean exist = sheets.Contains("管子开孔表");
            if (exist){
                uint total_row = Convert.ToUInt32(pipe_HoleList.Rows.Count);
                uint total_column = Convert.ToUInt32(subReportInfo[0, 2]);
                uint start_row = Convert.ToUInt32(subReportInfo[0, 0]);
                uint start_column = Convert.ToUInt32(subReportInfo[0, 1]);
                if (total_row > 0){
                    WriteDataToReport("管子开孔表", pipe_HoleList, report, total_row, total_column, start_row, start_column);
                    WriteTitleToReport("管子开孔表", pipe_TitleList, report, subTitleInfo);
                }
            }
            else{
                MessageBox.Show("模板文件中没有《管子开孔表》数据页，无法生成这个报表", "错误", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
            return;
        }

        public static void WriteTitleToReport(string reportName, DataTable table, string report, int[,] subTitleInfo)
        {
            using (SpreadsheetDocument document = SpreadsheetDocument.Open(report, true))
            {
                IEnumerable<Sheet> s = document.WorkbookPart.Workbook.Descendants<Sheet>().Where(x => x.Name == reportName);
                WorksheetPart workSheetPart = (WorksheetPart)document.WorkbookPart.GetPartById(s.First().Id);
                Worksheet worksheet = workSheetPart.Worksheet;
                SheetData sheetData = worksheet.GetFirstChild<SheetData>();

                for (int i = 0; i < subTitleInfo.GetLength(0); i++){
                    uint rowIndex = Convert.ToUInt32(subTitleInfo[i, 0]);
                    Row row = workSheetPart.Worksheet.GetFirstChild<SheetData>().Elements<Row>().Where(r => r.RowIndex == rowIndex).First();

                    Int32 colIndex = subTitleInfo[i, 1];
                    string columnName = NumToChar(colIndex);
                    string cellReference = columnName + rowIndex;       //确定单元格的位置
                    string data = table.Rows[0][i].ToString();          //获取单元格的内容
                    
                    //修正单元格的值
                    if (i == 0){
                        data = "工程编号:" + data;
                    }
                    else if (i == 2){
                        data = "区域:" + data;
                    }
                    else if (i == 3){
                        data = "托盘号:" + data;
                    }

                    //将数据写入单元格
                    try{
                        //如果字段值为空字符串，则跳过
                        if (data != ""){
                            // If the worksheet does not contain a row with the specified row index, insert one.
                            Row row1;
                            if (sheetData.Elements<Row>().Where(r => r.RowIndex == rowIndex).Count() != 0){
                                row1 = sheetData.Elements<Row>().Where(r => r.RowIndex == rowIndex).First();
                            }
                            else{
                                row1 = new Row() { RowIndex = rowIndex };
                                sheetData.Append(row);
                            }

                            // If there is not a cell with the specified column name, insert one.  
                            Cell newCell;
                            if (row1.Elements<Cell>().Where(c => c.CellReference.Value == columnName + rowIndex).Count() > 0){
                                newCell = row1.Elements<Cell>().Where(c => c.CellReference.Value == cellReference).First();
                            }
                            else{
                                // Cells must be in sequential order according to CellReference. Determine where to insert the new cell.
                                Cell refCell = null;
                                foreach (Cell cell in row1.Elements<Cell>()){
                                    if (cell.CellReference.Value.Length == cellReference.Length){
                                        if (string.Compare(cell.CellReference.Value, cellReference, true) > 0){
                                            refCell = cell;
                                            break;
                                        }
                                    }
                                }
                                newCell = new Cell() { CellReference = cellReference };
                                row.InsertBefore(newCell, refCell);
                            }
                            newCell.DataType = CellValues.InlineString;
                            newCell.InlineString = new InlineString() { Text = new Text(data) };
                        }
                    }
                    catch{
                        MessageBox.Show("错误", "无法将数据写入Excel表格", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    }
                }
                workSheetPart.Worksheet.Save();
                document.Close();
                document.Dispose();
            }
            return;
        }

        public static void WriteDataToReport(string reportName, DataTable table, string report, uint total_row, uint total_column, uint start_row, uint start_column)
        {
            using (SpreadsheetDocument document = SpreadsheetDocument.Open(report, true))
            {
                IEnumerable<Sheet> s = document.WorkbookPart.Workbook.Descendants<Sheet>().Where(x => x.Name == reportName);
                WorksheetPart workSheetPart = (WorksheetPart)document.WorkbookPart.GetPartById(s.First().Id);
                Worksheet worksheet = workSheetPart.Worksheet;
                SheetData sheetData = worksheet.GetFirstChild<SheetData>();
                for (int i = 0; i < total_row; i++){
                    Row row = workSheetPart.Worksheet.GetFirstChild<SheetData>().Elements<Row>().Where(r => r.RowIndex == start_row + i).First();

                    uint rowIndex = Convert.ToUInt32(start_row + i);
                    for (int j = 0; j < total_column; j++){
                        Int32 colIndex = Convert.ToInt32(start_column) + j;
                        string columnName = NumToChar(colIndex);
                        string cellReference = columnName + rowIndex;       //确定单元格的位置
                        string data = table.Rows[i][j].ToString();          //获取单元格的内容
                        //管段如果管段号相同，将管段号置为空字符串
                        if (reportName == "管子制造清册"){
                            if (i > 0 & j == 1){
                                string prev_data = table.Rows[i - 1][j].ToString();
                                if (prev_data == data){
                                    data = "";
                                }
                            }
                        }
                        //如果字段内容为“0”，将字段设置为空字符串
                        if (data == "0" | data == "0.0" | data == "0.00" | data == "Unset"){
                            data = "";
                        }
                        //单元格换行
                        if (reportName == "管子开孔表"){
                            data = data.Replace(",", Environment.NewLine);
                        }
                        
                        //将数据写入单元格
                        try{
                            //如果字段值为空字符串，则跳过
                            if (data != ""){
                                // If the worksheet does not contain a row with the specified row index, insert one.
                                Row row1;
                                if (sheetData.Elements<Row>().Where(r => r.RowIndex == rowIndex).Count() != 0){
                                    row1 = sheetData.Elements<Row>().Where(r => r.RowIndex == rowIndex).First();
                                }
                                else{
                                    row1 = new Row() { RowIndex = rowIndex };
                                    sheetData.Append(row);
                                }

                                // If there is not a cell with the specified column name, insert one.  
                                Cell newCell;
                                if (row1.Elements<Cell>().Where(c => c.CellReference.Value == columnName + rowIndex).Count() > 0){
                                    newCell = row1.Elements<Cell>().Where(c => c.CellReference.Value == cellReference).First();
                                }
                                else{
                                    // Cells must be in sequential order according to CellReference. Determine where to insert the new cell.
                                    Cell refCell = null;
                                    foreach (Cell cell in row1.Elements<Cell>()){
                                        if (cell.CellReference.Value.Length == cellReference.Length){
                                            if (string.Compare(cell.CellReference.Value, cellReference, true) > 0){
                                                refCell = cell;
                                                break;
                                            }
                                        }
                                    }
                                    newCell = new Cell() { CellReference = cellReference };
                                    row.InsertBefore(newCell, refCell);
                                }
                                //InlineString inlineString = new InlineString();
                                //Text txt = new Text();
                                //txt.Text = data;
                                //inlineString.AppendChild(txt);
                                //newCell.AppendChild(inlineString);
                                newCell.DataType = CellValues.InlineString;
                                newCell.InlineString = new InlineString() { Text = new Text(data) };
                            }
                        }
                        catch{
                            MessageBox.Show("错误", "无法将数据写入Excel表格", MessageBoxButtons.OK, MessageBoxIcon.Error);
                        }
                    }
                }
                workSheetPart.Worksheet.Save();
                document.Close();
                document.Dispose();
            }
            return;
        }

        public static string NumToChar(int number)
        {
            if (1 <= number && 36 >= number){
                int num = number + 64;
                System.Text.ASCIIEncoding asciiEncoding = new System.Text.ASCIIEncoding();
                byte[] btNumber = new byte[] { (byte)num };
                return asciiEncoding.GetString(btNumber);
            }
            else if (65 <= number && 90 >= number){
                System.Text.ASCIIEncoding asciiEncoding = new System.Text.ASCIIEncoding();
                byte[] btNumber = new byte[] { (byte)number };
                return asciiEncoding.GetString(btNumber);
            }
            else{
                return ("");
            }
        }

        public static List<String> GetSheets(String strFileName)
        {
            //  Fill this collection with a list of all the sheets.
            List<String> sheets = new List<String>();
            using (SpreadsheetDocument xlPackage = SpreadsheetDocument.Open(strFileName, false))
            {
                WorkbookPart workbook = xlPackage.WorkbookPart;
                Stream workbookstr = workbook.GetStream();
                XmlDocument doc = new XmlDocument();
                doc.Load(workbookstr);
                XmlNamespaceManager nsManager = new XmlNamespaceManager(doc.NameTable);
                nsManager.AddNamespace("default", doc.DocumentElement.NamespaceURI);
                XmlNodeList nodelist = doc.SelectNodes("//default:sheets/default:sheet", nsManager);

                foreach (XmlNode node in nodelist){
                    String sheetName = String.Empty;
                    sheetName = node.Attributes["name"].Value;
                    sheets.Add(sheetName);
                }
            }
            return sheets;
        }
    }
}
