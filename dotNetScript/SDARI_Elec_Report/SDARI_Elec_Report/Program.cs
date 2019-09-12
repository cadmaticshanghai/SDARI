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


namespace SDARI_Elec_Report
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
            if (reportName == "电气设备托盘表"){
                ElecEquipmentReport(Data, TargetFile, reportName);
            }
            else if (reportName == "电气舾装件托盘表"){
                ElecStructureReport(Data, TargetFile, reportName);
            }
            else if (reportName == "电气设备基座制作图册"){
                ElecFoundationReport(Data, TargetFile, reportName);
            }
            else if (reportName == "电缆支撑件和贯通件制作图册"){
                ElecCableTrayReport(Data, TargetFile, reportName);
            }

            //打开报表文件
            try{
                System.Diagnostics.Process.Start(TargetFile);
            }
            catch{
                return;
            }
        }

        //电气设备基座制作图册
        public static void ElecFoundationReport(string[] datas, string report, string reportName)
        {
            string sheetName1 = "电气设备基座制作图册";
            GetSheetReport(datas, report, reportName, sheetName1);
        }

        //电缆支撑件和贯通件制作图册
        public static void ElecCableTrayReport(string[] datas, string report, string reportName)
        {
            string sheetName1 = "电缆支撑件和贯通件制作图册";
            GetSheetReport(datas, report, reportName, sheetName1);
        }

        //电气设备托盘表
        public static void ElecEquipmentReport(string[] datas, string report, string reportName)
        {
            string sheetName1 = "电气设备托盘表";
            GetSheetReport(datas, report, reportName, sheetName1);
        }

        //电气舾装件托盘表
        public static void ElecStructureReport(string[] datas, string report, string reportName)
        {
            string sheetName1 = "托架";
            string sheetName2 = "扁钢";
            string sheetName3 = "贯通件";
            string sheetName4 = "支撑件";
            string sheetName5 = "设备基座";

            GetSheetReport(datas, report, reportName, sheetName1);
            GetSheetReport(datas, report, reportName, sheetName2);
            GetSheetReport(datas, report, reportName, sheetName3);
            GetSheetReport(datas, report, reportName, sheetName4);
            GetSheetReport(datas, report, reportName, sheetName5);
        }

        //获取Sheet页面数据
        public static DataTable GetSheetCellDataInfo(string[] datas, string sheetName, int totalColumn)
        {
            string[] seperator = { ",#" };                  //分隔符
            DataTable sheetCellDataList = new DataTable();  //托盘表数据信息  
            for (int i = 0; i < totalColumn; i++){
                sheetCellDataList.Columns.Add(i.ToString(), typeof(string));
            }

            Boolean flag = false;
            foreach (string line in datas){
                string[] elements = (string[])line.Split(seperator, StringSplitOptions.None);
                string element0 = elements[0];
                if (element0 == "Table"){//Sheet标识符
                    string element1 = elements[1];
                    if (element1 == sheetName){
                        flag = true;
                        continue;
                    }
                }

                if(flag){
                    if (element0 == "Line"){//数据标识符
                        AppendLineToDataTable(elements, sheetCellDataList);
                    }
                    else {
                        flag = false;
                        break;
                    }
                }
            }
            return sheetCellDataList;
        }

        //获取Excel页面信息
        public static DataTable GetReportTitleInfo(string[] datas, string excelName)
        {
            string[] seperator = { ",#" };              //分隔符
            DataTable sheetTitleList = new DataTable(); //托盘表信息  
            for (int i = 0; i < 3; i++){
                sheetTitleList.Columns.Add(i.ToString(), typeof(string));
            }

            Boolean flag = false;
            foreach (string line in datas){
                string[] elements = (string[])line.Split(seperator, StringSplitOptions.None);
                string element0 = elements[0];
                if (element0 == excelName){
                    flag = true;
                    continue;
                }
                if (flag){
                    if (element0 == "Title"){//标题标识符
                        AppendLineToDataTable(elements, sheetTitleList);
                    }
                    else {
                        flag = false;
                        break;
                    }
                }
            }
            return sheetTitleList;
        }

        //获取Sheet页面信息
        public static void GetSheetInfo(string[] datas, string sheetName, out int startRow, out int startColumn, out int totalColumn)
        {
            startRow = 0; startColumn = 0; totalColumn = 0;

            string subReport = "";
            string[] seperator = { ",#" }; //分隔符
            foreach (string line in datas){
                string[] elements = (string[])line.Split(seperator, StringSplitOptions.None);
                string element0 = elements[0];
                if (element0 == "Table"){//Sheet标识符
                    subReport = elements[1];
                    if (subReport == sheetName){
                        startRow = Convert.ToInt32(elements[2]);
                        startColumn = Convert.ToInt32(elements[3]);
                        totalColumn = Convert.ToInt32(elements[4]);
                        break;
                    }
                }
            }
            return;
        }

        //生成Sheet内容
        public static void GetSheetReport(string[] datas, string report, string excelName, string sheetName)
        {
            //用于定义Sheet数据表的位置
            int startRow; int startColumn; int totalColumn;
            GetSheetInfo(datas, sheetName, out startRow, out startColumn, out totalColumn);

            //用于Sheet固定单元格数据（图名图号等）
            DataTable sheet_TitleList = new DataTable();
            sheet_TitleList = GetReportTitleInfo(datas, excelName);

            //定义Sheet单元格数据
            DataTable sheet_DataList = new DataTable();
            sheet_DataList = GetSheetCellDataInfo(datas, sheetName, totalColumn);

            //对报表进行排序
            //if (sheet_DataList.Rows.Count > 1){
            //    sheet_DataList.DefaultView.Sort = "1";
            //    sheet_DataList = sheet_DataList.DefaultView.ToTable();
            //    for (int i = 0; i < sheet_DataList.Rows.Count; i++){
            //        sheet_DataList.Rows[i][0] = Convert.ToString(i + 1);
            //    }
            //}

            // 检查报表模板
            List<string> sheets = GetSheets(report);

            // 写数据到表格
            Boolean exist = sheets.Contains(sheetName);
            if (exist){
                int totalRow = Convert.ToInt32(sheet_DataList.Rows.Count);
                if (totalRow > 0){
                    WriteDataToReport(sheetName, sheet_DataList, report, totalRow, totalColumn, startRow, startColumn);
                    WriteTitleToReport(sheetName, sheet_TitleList, report);
                }
            }
            else{
                string msg = "模板文件中没有《" + sheetName + "》数据页，无法生成这个报表";
                MessageBox.Show(msg, "错误", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
            return;
        }

        public static void AppendLineToDataTable(string[] elements, DataTable table)
        {
            DataRow dr = table.NewRow();
            int length = elements.Length;
            string[] datas = new string[length - 2];
            Array.Copy(elements, 1, datas, 0, length - 2);
            dr.ItemArray = datas;
            table.Rows.Add(dr);
        }

        public static void WriteTitleToReport(string sheetName, DataTable table, string report)
        {
            using (SpreadsheetDocument document = SpreadsheetDocument.Open(report, true))
            {
                IEnumerable<Sheet> s = document.WorkbookPart.Workbook.Descendants<Sheet>().Where(x => x.Name == sheetName);
                WorksheetPart workSheetPart = (WorksheetPart)document.WorkbookPart.GetPartById(s.First().Id);
                Worksheet worksheet = workSheetPart.Worksheet;
                SheetData sheetData = worksheet.GetFirstChild<SheetData>();

                uint colCount = Convert.ToUInt32(table.Columns.Count);
                uint rowCount = Convert.ToUInt32(table.Rows.Count);
                for (int i = 0; i < rowCount; i++){
                    string data = table.Rows[i][0].ToString();          //获取单元格的内容
                    //修正单元格的值

                    Int32 colIndex = Convert.ToInt32(table.Rows[i][2]);
                    string colName = NumToChar(colIndex); ;
                    string rowName = table.Rows[i][1].ToString();
                    string cellReference = colName + rowName;           //确定单元格的位置

                    uint rowIndex = Convert.ToUInt32(table.Rows[i][1]);
                    Row row = workSheetPart.Worksheet.GetFirstChild<SheetData>().Elements<Row>().Where(r => r.RowIndex == rowIndex).First();

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
                            if (row1.Elements<Cell>().Where(c => c.CellReference.Value == cellReference).Count() > 0){
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

        public static void WriteDataToReport(string sheetName, DataTable table, string report, int total_row, int total_column, int start_row, int start_column)
        {
            using (SpreadsheetDocument document = SpreadsheetDocument.Open(report, true))
            {
                IEnumerable<Sheet> s = document.WorkbookPart.Workbook.Descendants<Sheet>().Where(x => x.Name == sheetName);
                WorksheetPart workSheetPart = (WorksheetPart)document.WorkbookPart.GetPartById(s.First().Id);
                Worksheet worksheet = workSheetPart.Worksheet;
                SheetData sheetData = worksheet.GetFirstChild<SheetData>();
                for (int i = 0; i < total_row; i++){
                    uint rowIndex = Convert.ToUInt32(start_row + i);
                    Row row = workSheetPart.Worksheet.GetFirstChild<SheetData>().Elements<Row>().Where(r => r.RowIndex == rowIndex).First();
                    for (int j = 0; j < total_column; j++){
                        Int32 colIndex = Convert.ToInt32(start_column) + j;
                        string columnName = NumToChar(colIndex);
                        string cellReference = columnName + rowIndex;       //确定单元格的位置
                        string data = table.Rows[i][j].ToString();          //获取单元格的内容
                        //管段如果管段号相同，将管段号置为空字符串
                        if (sheetName == "管子制造清册"){
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
                        if (sheetName == "管子开孔表"){
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

        public static List<String> GetSheets(String strFileName)
        {
            //  Fill this collection with a list of all the sheets.
            List<String> sheets = new List<String>();
            using (SpreadsheetDocument xlPackage = SpreadsheetDocument.Open(strFileName, false)){
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

    }
}
