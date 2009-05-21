
class EgtGpsDataTable : QTableWidget
{

%TypeHeaderCode
#include <egtgpsdatatable.h>
%End

  public:

     /*! \brief Constructor */
    EgtGpsDataTable( );

    /*! \brief Resets a column header */
    void clearColumnHeader( int );

    /*! \brief Indicates if the user has set at least column header */
    bool isAnyColumnHeaderSet( );

    /*! \brief Indicates if a specific column header has been set */
    bool isColumnHeaderSet( QString );

    /*! \brief Returns a QMap that contains all the key-value pairs of the selected row */
    QMap<QString,QString> rowData( );

    /*! \brief Returns a QMap that contains all the key-value pairs of the specified row */
    QMap<QString,QString> rowData( int );

    /*! \brief Set the column header for a specified row */
    bool setColumnHeader( int, QString );

  public slots:

    /*! \brief Broadcasts the key-value pairs in the currently selected row via the application interface */
    void broadcastRow( );

    /*! \brief Broadcasts the key-value pairs in the specified row via the application interface */
    void broadcastRow( int );

    /*! \brief Deletes the current row of the table */
    void deleteRow( );

    /*! \brief Deletes a row of the table */
    void deleteRow( int );

    /*! \brief Select a row and make it the current row */
    void selectRow( int );

    /*! \brief Sets the offset*/
    void setOffset( int );

    /*! \brief Set the table's data provider */
    void setProvider( EgtDataProvider* );

};
