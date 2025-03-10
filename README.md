# Convert XML to XSD

This is a experimental project to convert XML to XSD schema using C.
The idea is to practice the C language, plus creating a usefull tool for the day to day of data integration.

Example input:

```xml
<?xml version="1.0" encoding="UTF-8"?>
<root>
    <childrenRoot>
           <elementOne>teste</elementOne>
   </childrenRoot>
    <element1>value1</element1>
    <element2>value2</element2>
    <element3>value3</element3>
</root>
```

Expected output:

```xsd
<xs:schema attributeFormDefault="unqualified" elementFormDefault="qualified" xmlns:xs="http://www.w3.org/2001/XMLSchema">
  <xs:element name="root">
    <xs:complexType>
      <xs:sequence>
        <xs:element name="childrenRoot">
          <xs:complexType>
            <xs:sequence>
              <xs:element type="xs:string" name="elementOne"/>
            </xs:sequence>
          </xs:complexType>
        </xs:element>
        <xs:element type="xs:string" name="element1"/>
        <xs:element type="xs:string" name="element2"/>
        <xs:element type="xs:string" name="element3"/>
      </xs:sequence>
    </xs:complexType>
  </xs:element>
</xs:schema>
```

<xs:schema attributeFormDefault="unqualified" elementFormDefault="qualified" xmlns:xs="http://www.w3.org/2001/XMLSchema">
<xs:element name="root">
<xs:complexType>
<xs:sequence>
<xs:element name="childrenRoot">
<xs:complexType>
<xs:sequence>
<xs:element type="xs:string" name="elementOne"/>
</xs:sequence>
</xs:complexType>
</xs:element>
</xs:sequence>
</xs:complexType>
</xs:element>
</xs:schema>
